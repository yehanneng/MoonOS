//
// Created by Tangruiwen on 2017/3/4.
//

#include "harddisktask.h"
#include <kernel/kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


uint8_t ide_buffer[2048];

HardDiskTask::HardDiskTask()
:ata_pm(false),ata_ps(false),ata_sm(false),ata_ss(false)
{
    memset(_drivers, 0, 4 * sizeof(ide_private_data));
}

HardDiskTask::~HardDiskTask() {

}

void HardDiskTask::run() {
    printf("Hard Disk Task begin run\n");
    ata_probe();
    if (ata_pm) {
        printf("HD info: \n");
        printf("HD size = %d \n", _drivers[0].size * 512 / 1024 / 1024);
        printf("mode = %s \n", _drivers[0].model);
    }
    while(1){}
}

void HardDiskTask::ide_select_drive(uint8_t bus, uint8_t i) {
    if(bus == ATA_PRIMARY) {
        if (i == ATA_MASTER) {
            out_byte(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
        } else {
            out_byte(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
        }
    } else {
        if (i == ATA_MASTER) {
            out_byte(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
        }
        else {
            out_byte(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
        }
    }
}

uint8_t HardDiskTask::ide_identify(uint8_t bus, uint8_t drive) {
    uint16_t io = 0;
    ide_select_drive(bus, drive);
    if(bus == ATA_PRIMARY) io = ATA_PRIMARY_IO;
    else io = ATA_SECONDARY_IO;
    /* ATA specs say these values must be zero before sending IDENTIFY */
    out_byte(io + ATA_REG_SECCOUNT0, 0);
    out_byte(io + ATA_REG_LBA0, 0);
    out_byte(io + ATA_REG_LBA1, 0);
    out_byte(io + ATA_REG_LBA2, 0);
    /* Now, send IDENTIFY */
    out_byte(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    printf("Sent IDENTIFY\n");
    /* Now, read status port */
    uint8_t status = in_byte(io + ATA_REG_STATUS);
    if(status)
    {
        /* Now, poll untill BSY is clear. */
        while(in_byte(io + ATA_REG_STATUS) & ATA_SR_BSY != 0) ;
pm_stat_read:
        status = in_byte(io + ATA_REG_STATUS);
        if(status & ATA_SR_ERR)
        {
            printf("%d | %d has ERR set. Disabled.\n", bus, drive);
            return 0;
        }
        while(!(status & ATA_SR_DRQ)) goto pm_stat_read;
        printf("%s%s is online.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
        /* Now, actually read the data */

        for(int i = 0; i<256; i++)
        {
            *(uint16_t *)(ide_buffer + i*2) = port_read(io + ATA_REG_DATA);
        }
    }
    return 1;
}

void HardDiskTask::ide_400ns_delay(uint16_t io) {
    for(int i = 0;i < 4; i++)
        in_byte(io + ATA_REG_ALTSTATUS);
}

void HardDiskTask::ide_poll(uint16_t io) {
    for(int i=0; i< 4; i++)
        in_byte(io + ATA_REG_ALTSTATUS);

retry:
    uint8_t status = in_byte(io + ATA_REG_STATUS);
    //mprint("testing for BSY\n");
    if(status & ATA_SR_BSY) goto retry;
    //mprint("BSY cleared\n");
retry2:
    status = in_byte(io + ATA_REG_STATUS);
    if(status & ATA_SR_ERR)
    {
        printf("ERR set, device failure!\n");
        //abort();
    }
    //mprint("testing for DRQ\n");
    if(!(status & ATA_SR_DRQ)) goto retry2;
    //mprint("DRQ set, ready to PIO!\n");
    return;
}

void HardDiskTask::wait_ide_interrupt() {
    send_recv(RECEIVE, INTERRUPT, &this->_msg);
}

void HardDiskTask::ata_probe() {
    /* First check the primary bus,
	 * and inside the master drive.
	 */
    int ret = ide_identify(ATA_PRIMARY, ATA_MASTER);
    if(ret)
    {
        ata_pm = true;
        ide_private_data *priv = &_drivers[0];
        priv->commandSets = *((unsigned int *)(ide_buffer + ATA_IDENT_COMMANDSETS));
        priv->drive = (ATA_PRIMARY << 1) | ATA_MASTER;

        if (priv->commandSets & (1 << 26)){
            // Device uses 48-Bit Addressing:
            priv->size = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA_EXT));
        } else {
            // Device uses CHS or 28-bit Addressing:
            priv->size = *((unsigned int *)(ide_buffer + ATA_IDENT_MAX_LBA));
        }
        for(int k = 0; k < 40; k += 2) {
            priv->model[k] = ide_buffer[ATA_IDENT_MODEL + k + 1];
            priv->model[k + 1] = ide_buffer[ATA_IDENT_MODEL + k];
        }
        priv->model[40] = 0; // Terminate String.

    }
    ide_identify(ATA_PRIMARY, ATA_SLAVE);
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

