//
// Created by Tangruiwen on 2017/3/4.
//

#include "harddisktask.h"
#include <kernel/kernel.h>
#include <stdint.h>
#include <stdio.h>

IDEChannelRegisters Channels[2];
IDE_device ide_devices[4];
uint8_t ide_buffer[2048];
uint32_t ide_irq_invoked = 0;
uint8_t atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

HardDiskTask::HardDiskTask(){

}

HardDiskTask::~HardDiskTask() {

}

void HardDiskTask::run() {
    printf("Hard Disk Task begin run\n");
    ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
    while(1){}
}

uint8_t HardDiskTask::ide_read(uint8_t channel, uint8_t reg) {
    uint8_t result = 0;

    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | Channels[channel].nIEN);
    if (reg < 0x08)
        result = in_byte(Channels[channel].base + reg - 0x00);
    else if (reg < 0x0C)
        result = in_byte(Channels[channel].base  + reg - 0x06);
    else if (reg < 0x0E)
        result = in_byte(Channels[channel].ctrl  + reg - 0x0A);
    else if (reg < 0x16)
        result = in_byte(Channels[channel].bmide + reg - 0x0E);
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, Channels[channel].nIEN);
    return result;
}

void HardDiskTask::ide_write(uint8_t channel, uint8_t reg, uint8_t data) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | Channels[channel].nIEN);
    if (reg < 0x08)
        out_byte(Channels[channel].base  + reg - 0x00, data);
    else if (reg < 0x0C)
        out_byte(Channels[channel].base  + reg - 0x06, data);
    else if (reg < 0x0E)
        out_byte(Channels[channel].ctrl  + reg - 0x0A, data);
    else if (reg < 0x16)
        out_byte(Channels[channel].bmide + reg - 0x0E, data);
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, Channels[channel].nIEN);
}

void HardDiskTask::ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t buffer, uint32_t quads) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, 0x80 | Channels[channel].nIEN);
    if (reg < 0x08)
        read_port_to_buffer(Channels[channel].base  + reg - 0x00, (void*)buffer, quads);
    else if (reg < 0x0C)
        read_port_to_buffer(Channels[channel].base  + reg - 0x06, (void*)buffer, quads);
    else if (reg < 0x0E)
        read_port_to_buffer(Channels[channel].ctrl  + reg - 0x0A, (void*)buffer, quads);
    else if (reg < 0x16)
        read_port_to_buffer(Channels[channel].bmide + reg - 0x0E, (void*)buffer, quads);
    if (reg > 0x07 && reg < 0x0C)
        ide_write(channel, ATA_REG_CONTROL, Channels[channel].nIEN);
}

uint8_t HardDiskTask::ide_polling(uint8_t channel, uint32_t advanced_check) {
    // (I) Delay 400 nanosecond for BSY to be set:
    // -------------------------------------------------
    for(int i = 0; i < 4; i++)
        ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times
    // (II) Wait for BSY to be cleared:
    // -------------------------------------------------
    wait_ide_interrupt();
    while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
        ; // Wait for BSY to be zero.

    if (advanced_check) {
        unsigned char state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.

        // (III) Check For Errors:
        // -------------------------------------------------
        if (state & ATA_SR_ERR)
            return 2; // Error.

        // (IV) Check If Device fault:
        // -------------------------------------------------
        if (state & ATA_SR_DF)
            return 1; // Device Fault.

        // (V) Check DRQ:
        // -------------------------------------------------
        // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
        if ((state & ATA_SR_DRQ) == 0)
            return 3; // DRQ should be set

    }

    return 0; // No Error.
}

uint8_t HardDiskTask::ide_print_error(uint32_t driver, uint8_t err) {
    return err;
}

void HardDiskTask::ide_initialize(uint32_t bar0, uint32_t bar1, uint32_t bar2, uint32_t bar3, uint32_t bar4) {
    int i,j, k, count = 0;

    // 1- Detect I/O Ports which interface IDE Controller:
    Channels[ATA_PRIMARY  ].base  = (bar0 & 0xFFFFFFFC) + 0x1F0 * (!bar0);
    Channels[ATA_PRIMARY  ].ctrl  = (bar1 & 0xFFFFFFFC) + 0x3F6 * (!bar1);
    Channels[ATA_SECONDARY].base  = (bar2 & 0xFFFFFFFC) + 0x170 * (!bar2);
    Channels[ATA_SECONDARY].ctrl  = (bar3 & 0xFFFFFFFC) + 0x376 * (!bar3);
    Channels[ATA_PRIMARY  ].bmide = (bar4 & 0xFFFFFFFC) + 0; // Bus Master IDE
    Channels[ATA_SECONDARY].bmide = (bar4 & 0xFFFFFFFC) + 8; // Bus Master IDE

    // 2- Disable IRQs:
    ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
    ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    // 3- Detect ATA-ATAPI Devices:
    for (i = 0; i < 2; ++i) {
        for(j = 0; j < 2; j++) {
            uint8_t err, type = IDE_ATA, status;
            ide_devices[count].reserved = 0;    // at start, set all driver to 0

            // (I) Select Drive:
            ide_write(i ,ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            // wait for a interrupt
            // wait_ide_interrupt();


            // (II) Send ATA Identify Command:
            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            // wait for a interrupt
            // wait_ide_interrupt();

            // (III) Polling:
            if (ide_read(i, ATA_REG_STATUS) == 0){
                // If Status = 0, No Device.
                continue;
            }

            while(1) {
                status = ide_read(i, ATA_REG_STATUS);
                if ((status & ATA_SR_ERR)) {
                    // If Err, Device is not ATA.
                    printf("break point3 status = %d\n", status);
                    err = 1;
                    break;
                }
                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
            }

            // (IV) Probe for ATAPI Devices:
            if (err != 0) {
                unsigned char cl = ide_read(i, ATA_REG_LBA1);
                unsigned char ch = ide_read(i, ATA_REG_LBA2);

                if (cl == 0x14 && ch ==0xEB)
                    type = IDE_ATAPI;
                else if (cl == 0x69 && ch == 0x96)
                    type = IDE_ATAPI;
                else
                    continue; // Unknown Type (may not be a device).

                ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                wait_ide_interrupt();
            }

            // (V) Read Identification Space of the Device:
            ide_read_buffer(i, ATA_REG_DATA, (unsigned int) ide_buffer, 1);

            // (VI) Read Device Parameters:
            ide_devices[count].reserved     = 1;
            ide_devices[count].type         = type;
            ide_devices[count].channel      = i;
            ide_devices[count].driver        = j;
            ide_devices[count].signature    = *((unsigned short *)(ide_buffer + ATA_IDENT_DEVICETYPE));
            ide_devices[count].capabilities = *((unsigned short *)(ide_buffer + ATA_IDENT_CAPABILITIES));
            ide_devices[count].commandSets  = *((unsigned int *)(ide_buffer + ATA_IDENT_COMMANDSETS));

            // (VII) Get Size:
            if (ide_devices[count].commandSets & (1 << 26))
                // Device uses 48-Bit Addressing:
                ide_devices[count].size   = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA_EXT));
            else
                // Device uses CHS or 28-bit Addressing:
                ide_devices[count].size   = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA));

            // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
            for(k = 0; k < 40; k += 2) {
                ide_devices[count].model[k] = ide_buffer[ATA_IDENT_MODEL + k + 1];
                ide_devices[count].model[k + 1] = ide_buffer[ATA_IDENT_MODEL + k];
            }
            ide_devices[count].model[40] = 0; // Terminate String.

            count++;
        }
    }
    // 4- Print Summary:
    for (i = 0; i < 4; i++)
        if (ide_devices[i].reserved == 1) {
            printf(" Found %d Drive %dGB - %s\n",
                   ide_devices[i].type,         /* Type */
                    ide_devices[i].size / 1024 / 1024 / 2,               /* Size */
                    ide_devices[i].model);
        }
}

void HardDiskTask::wait_ide_interrupt() {
    send_recv(RECEIVE, INTERRUPT, &this->_msg);
}

/**
 *
 * @param reg
 * @param buffer
 * @param sectors num of sectors to read
 */
void HardDiskTask::read_port_to_buffer(uint8_t reg, void *buffer, uint32_t sectors) {
    uint16_t * p = (uint16_t *) buffer;

    for (int i = 0; i < sectors * SECTOR_SIZE / 2; ++i) {
        p[i] = port_read(reg);
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void hd_task_main()
{
    HardDiskTask _hard_task;
    _hard_task.run();
}

#ifdef __cplusplus
};
#endif

