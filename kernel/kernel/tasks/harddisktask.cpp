//
// Created by Tangruiwen on 2017/3/4.
//

#include "harddisktask.h"
#include <kernel/kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


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
    if (this->ata_pm) {
        printf("HD0 is avalible | size = %d \n", this->_drivers[0].size * 512 / 1024 / 1024);
    }
    while(1){
        int ret = send_recv(RECEIVE, ANY, &_msg);
        if (ret == 0) {
            int type = this->_msg.type;
            switch (type) {
                case HD_READ:
                {
                    int src = this->_msg.source;
                    void* read_buffer = kernel_va2la(src,this->_msg.BUF);
                    int begin_lba = this->_msg.START_LBA;
                    int sectors = this->_msg.SECTORS;
                    // for now hard code use hd0
                    int status = this->ata_read((uint8_t*)read_buffer, begin_lba, sectors, 0);
                    this->_msg.STATUS = status;
                    send_recv(SEND, src, &_msg);
                    printf("after read hd\n");
                    break;
                }
                default:
                    break;
            }
        }

    }
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
    // printf("Sent IDENTIFY\n");
    /* Now, read status port */
    uint8_t status = in_byte(io + ATA_REG_STATUS);
    if(status)
    {
        /* Now, poll untill BSY is clear. */
        int retry_time = 0;
        while(in_byte(io + ATA_REG_STATUS) & ATA_SR_BSY != 0){
            retry_time++;
            if (retry_time == 10) return 0;
        }
        retry_time = 0;
pm_stat_read:
        status = in_byte(io + ATA_REG_STATUS);
        if(status & ATA_SR_ERR)
        {
            printf("%d | %d has ERR set. Disabled.\n", bus, drive);
            return 0;
        }
        while(!(status & ATA_SR_DRQ)){
            retry_time++;
            if (retry_time == 10) return 0;
            goto pm_stat_read;
        }
        // printf("%s%s is online.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
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
    if(status & ATA_SR_BSY) goto retry;
retry2:
    status = in_byte(io + ATA_REG_STATUS);
    if(status & ATA_SR_ERR)
    {
        printf("ERR set, device failure!\n");
        abort();
    }
    if(!(status & ATA_SR_DRQ)) goto retry2;
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
        this->ata_pm = true;
        ide_private_data *priv = &_drivers[0];
        _save_device_info(priv);
    }
    ret = ide_identify(ATA_PRIMARY, ATA_SLAVE);
    if (ret) {
        this->ata_ps = true;
        ide_private_data* priv = &_drivers[1];
        _save_device_info(priv);
    }
    ret = ide_identify(ATA_SECONDARY, ATA_MASTER);
    if (ret) {
        this->ata_sm = true;
        ide_private_data* priv = &_drivers[2];
        _save_device_info(priv);
    }
    /*
    some thing wrong with QEMU secondary slaver IDE channel
    ret = ide_identify(ATA_SECONDARY, ATA_SLAVE);
    printf("ret = %d \n", ret);
    if (ret) {
        this->ata_ss = true;
        ide_private_data* priv = &_drivers[2];
        _save_device_info(priv);
    }
    */
}

/**
 * read one sector from hard disk
 * @param buf
 * @param lba
 * @param dev
 * @return
 */
uint8_t HardDiskTask::ata_read_one(uint8_t *buf, uint32_t lba, uint32_t dev) {
    uint8_t driver = _drivers[dev].drive;

    uint16_t io = 0;
    switch(driver)
    {
        case (ATA_PRIMARY << 1 | ATA_MASTER):
            io = ATA_PRIMARY_IO;
            driver = ATA_MASTER;
            break;
        case (ATA_PRIMARY << 1 | ATA_SLAVE):
            io = ATA_PRIMARY_IO;
            driver = ATA_SLAVE;
            break;
        case (ATA_SECONDARY << 1 | ATA_MASTER):
            io = ATA_SECONDARY_IO;
            driver = ATA_MASTER;
            break;
        case (ATA_SECONDARY << 1 | ATA_SLAVE):
            io = ATA_SECONDARY_IO;
            driver = ATA_SLAVE;
            break;
        default:
            return 0;
    }

    uint8_t cmd = (driver == ATA_MASTER?0xE0:0xF0);
    uint8_t slavebit = (driver == ATA_MASTER?0x00:0x01);
    out_byte(io + ATA_REG_HDDEVSEL, (cmd | (uint8_t)((lba >> 24 & 0x0F))));
    out_byte(io + 1, 0x00);
    out_byte(io + ATA_REG_SECCOUNT0, 1);
    out_byte(io + ATA_REG_LBA0, (uint8_t)((lba)));
    out_byte(io + ATA_REG_LBA1, (uint8_t)((lba) >> 8));
    out_byte(io + ATA_REG_LBA2, (uint8_t)((lba) >> 16));
    out_byte(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ide_poll(io);

    for(int i = 0; i < 256; i++)
    {
        uint16_t data = port_read(io + ATA_REG_DATA);
        *(uint16_t *)(buf + i * 2) = data;
    }
    ide_400ns_delay(io);
    return 1;
}

uint8_t HardDiskTask::ata_read(uint8_t *buf, uint32_t start_lba, uint32_t sectors, uint32_t dev) {
    uint8_t* p_buf = buf;
    uint32_t current_lba = start_lba;
    for (int i = 0; i < sectors; ++i) {
        if (this->ata_read_one(p_buf, current_lba, dev)) {
            p_buf += SECTOR_SIZE;
            current_lba++;
        } else {
            // some thing is wrong
            return 0;
        }
    }
    return 1;
}

void HardDiskTask::_save_device_info(ide_private_data* priv)
{
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

