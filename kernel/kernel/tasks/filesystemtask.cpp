//
// Created by Tangruiwen on 2017/3/8.
//

#include "harddisktask.h"
#include "filesystemtask.h"
#include <assert.h>
#include <stdio.h>

PartitionInfo::PartitionInfo()
:empty(true),boot_indicate(0),system_id(0),base_sectors(0),start_sectors(0),total_sectors(0)
{

}

PartitionInfo::~PartitionInfo()
{

}

FileSystemTask::FileSystemTask()
:empty_index(0)
{

}

FileSystemTask::~FileSystemTask()
{

}


void FileSystemTask::run() {
    this->prase_partition_info();
    PartitionInfo* p_info = get_partition_info(0);
    if (!p_info->empty) {
        // get one Partition Info
        printf("Partition Info : \n");
        printf("SystemID = %d | start_sector = %d | total_sector = %d\n", p_info->system_id, p_info->start_sectors, p_info->total_sectors);
    }
    while (1) {

    }
}

PartitionInfo* FileSystemTask::get_empty_partition_info() {
    return get_partition_info(empty_index);
}

PartitionInfo* FileSystemTask::get_partition_info(uint32_t index) {
    assert(index < MAX_PARTITIONS);
    return _partition_infos + index;
}

void FileSystemTask::prase_partition_info() {
    /* first read the MBR sector and get the root Partition Table */
    uint8_t buf[SECTOR_SIZE];
    _msg.type = HD_READ;
    _msg.START_LBA = 0; // first Partition Table always in Sector 0
    _msg.SECTORS = 1;
    _msg.BUF = buf;
    send_recv(BOTH, HD_DEST, &_msg);

    uint8_t * partition_one = buf + PARTITION_ONE_OFFSET;
    uint8_t system_id = *(partition_one + SYSTEM_ID_OFFSET);
    if (system_id == 0) { // no partition info
        return;
    }
    PartitionInfo* p_info = get_empty_partition_info();
    p_info->system_id = system_id;
    uint8_t bootinfo = *(partition_one + BOOT_INFO_OFFSET);
    p_info->boot_indicate = bootinfo;
    uint32_t* start_sector = (uint32_t*)(partition_one + START_SECTOR_OFFSET);
    p_info->start_sectors = *start_sector;
    uint32_t* total_sector = (uint32_t*)(partition_one + TOTAL_SECTOR_OFFSET);
    p_info->total_sectors = *total_sector;
    p_info->empty = false;

}


#ifdef __cplusplus
extern "C" {
#endif

void file_task_main()
{
    FileSystemTask _file_task;
    _file_task.run();
}

#ifdef __cplusplus
};
#endif