//
// Created by Tangruiwen on 2017/3/4.
//

#ifndef MOONOS_HDTASK_H
#define MOONOS_HDTASK_H

#include <kernel/tasks/task.h>
#include <stdint.h>
#include <stdlib.h>

/* MACRO Of IDE Driver */

#define SECTOR_SIZE		512
#define SECTOR_BITS		(SECTOR_SIZE * 8)

/* STATUS */
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Inlex
#define ATA_SR_ERR     0x01    // Error

/* ERROR */
#define ATA_ER_BBK      0x80    // Bad sector
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // No media
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // No media
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark

/* COMMAND */
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

/* The commands below are for ATAPI devices */
#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

/*
 * ATA_CMD_IDENTIFY_PACKET and ATA_CMD_IDENTIFY return a buffer of 512 bytes called the identification space;
 * the following definitions are used to read information from the identification space.
 * */

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

/* interface type and master or slave */
#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

/*
 * Task File is a range of 8 ports which are offsets from BAR0 (primary channel) and/or BAR2 (secondary channel). To exemplify:
 * BAR0 + 0 is first port.
 * BAR0 + 1 is second port.
 * BAR0 + 2 is the third
 * */
#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

/*
 * The ALTSTATUS/CONTROL port returns the alternate status when read and controls a channel when written to.
 * For the primary channel, ALTSTATUS/CONTROL port is BAR1 + 2.
 * For the secondary channel, ALTSTATUS/CONTROL port is BAR3 + 2.
 * We can now say that each channel has 13 registers. For the primary channel, we use these values:
 * Data Register: BAR0 + 0; // Read-Write
 * Error Register: BAR0 + 1; // Read Only
 * Features Register: BAR0 + 1; // Write Only
 * SECCOUNT0: BAR0 + 2; // Read-Write
 * LBA0: BAR0 + 3; // Read-Write
 * LBA1: BAR0 + 4; // Read-Write
 * LBA2: BAR0 + 5; // Read-Write
 * HDDEVSEL: BAR0 + 6; // Read-Write, used to select a drive in the channel.
 * Command Register: BAR0 + 7; // Write Only.
 * Status Register: BAR0 + 7; // Read Only.
 * Alternate Status Register: BAR1 + 2; // Read Only.
 * Control Register: BAR1 + 2; // Write Only.
 * DEVADDRESS: BAR1 + 3; // I don't know what is the benefit from this register.
 * */

// Channels:
#define      ATA_PRIMARY      0x00
#define      ATA_SECONDARY    0x01

// Directions:
#define      ATA_READ      0x00
#define      ATA_WRITE     0x01

#define     BAR0    0x1F0
#define     BAR1    0x3F6
#define     BAR2    0x170
#define     BAR3    0X376

// represent ide channel
struct _ide_channel_registers{
    uint16_t base; // I/O port base
    uint16_t ctrl; // Control port Base
    uint16_t bmide; // Bus Master IDE
    uint8_t nIEN; // No Interrupt
};

typedef struct _ide_channel_registers IDEChannelRegisters;
// each one represent a ide devices
struct _ide_devices{
    uint8_t reserved;   //0 is Empty, 1 The Driver exist
    uint8_t channel;    //0 primary channel, 1 secondary channel
    uint8_t driver;     //0 Master driver, 1 Slaver driver
    uint16_t type;      //0 ATA , 1 ATAPI
    uint16_t signature; //Driver Signature
    uint16_t capabilities; // Features
    uint32_t commandSets;   // Command Sets Support
    uint32_t size;      // size in sectors
    uint8_t model[4];      // Model in string
};

typedef struct _ide_devices IDE_device;



#ifdef __cplusplus

/**
 * Class of Hard Disk Task
 */
class HardDiskTask: public AbstractTask {
public:
    HardDiskTask();
    virtual ~HardDiskTask();
    virtual void run() override ;

private:
    uint8_t ide_read(uint8_t channel, uint8_t reg);
    void ide_write(uint8_t channel, uint8_t reg, uint8_t data);
    void ide_read_buffer(uint8_t channel, uint8_t reg, uint32_t buffer, uint32_t quads);
    uint8_t ide_polling(uint8_t channel, uint32_t advanced_check);
    uint8_t ide_print_error(uint32_t driver, uint8_t err);
    void ide_initialize(uint32_t bar0, uint32_t bar1, uint32_t bar2, uint32_t bar3, uint32_t bar4);
    void wait_ide_interrupt();
    void read_port_to_buffer(uint8_t reg, void* buffer, uint32_t size);
private:
    MESSAGE _msg;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

void hd_task_main();

#ifdef __cplusplus
};
#endif

#endif //MOONOS_HDTASK_H
