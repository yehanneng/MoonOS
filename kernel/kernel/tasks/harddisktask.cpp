//
// Created by Tangruiwen on 2017/3/4.
//

#include "harddisktask.h"
#include <kernel/kernel.h>
#include <stdint.h>

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

}

uint8_t HardDiskTask::ide_read(uint8_t channel, uint8_t reg) {
    uint8_t result;

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

}

uint8_t HardDiskTask::ide_polling(uint8_t channel, uint32_t advance_chack) {
    return 0;
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

            // Select the driver
            ide_write(i ,ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            // wait for a interrupt


            // send ATA identify command
            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            // wait for a interrupt
        }
    }
}