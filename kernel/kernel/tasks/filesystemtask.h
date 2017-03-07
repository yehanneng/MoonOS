//
// Created by Tangruiwen on 2017/3/8.
//

#ifndef MOONOS_FILESYSTEMTASK_H
#define MOONOS_FILESYSTEMTASK_H

#include <kernel/kernel.h>
#include <kernel/tasks/task.h>
#include <stdint.h>
#include <stdlib.h>

/* MARCOS of Partition Info Offset */
#define PARTITION_ONE_OFFSET    0x01BE
#define PARTITION_TWO_OFFSET    0x01CE
#define PARTITION_THREE_OFFSET  0x01DE
#define PARTITION_FOUR_OFFSET   0x01EE

#define BOOT_INFO_OFFSET 0
#define SYSTEM_ID_OFFSET 4
#define START_SECTOR_OFFSET 8
#define TOTAL_SECTOR_OFFSET 12


#define MAX_PARTITIONS 16
#define HAS_BOOT_CODE 0x80
#define NO_BOOT_CODE 0x00

#ifdef __cplusplus

class PartitionInfo{
public:
    PartitionInfo();
    virtual ~PartitionInfo();

public:
    bool empty;
    uint8_t boot_indicate;
    uint8_t system_id;
    uint32_t base_sectors;
    uint32_t start_sectors;
    uint32_t total_sectors;
};

class FileSystemTask: public AbstractTask {
public:
    FileSystemTask();
    virtual ~FileSystemTask();
    virtual void run() override ;

private:
    void prase_partition_info();
    PartitionInfo* get_empty_partition_info();
    PartitionInfo* get_partition_info(uint32_t index);
private:
    uint32_t empty_index;
    PartitionInfo _partition_infos[MAX_PARTITIONS];
    MESSAGE _msg;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

void file_task_main();

#ifdef __cplusplus
};
#endif

#endif //MOONOS_FILESYSTEMTASK_H
