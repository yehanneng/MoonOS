//
// Created by Tangruiwen on 2017/3/8.
//

#include "harddisktask.h"
#include "filesystemtask.h"
#include <assert.h>
#include <stdio.h>
#include <liballoc.h>
#include <string.h>

PartitionInfo::PartitionInfo()
:empty(true),boot_indicate(0),system_id(0),base_sectors(0),start_sectors(0),total_sectors(0)
,_parent_partition(nullptr)
{

}

PartitionInfo::~PartitionInfo()
{

}

uint32_t PartitionInfo::getAbsStartSector() {
    PartitionInfo* p = this->_parent_partition;
    uint32_t ret = this->start_sectors;
    while(p != nullptr) {
        ret += p->start_sectors;
        p = p->_parent_partition;
    }
    return ret;
}

FileSystemTask::FileSystemTask()
:empty_index(0)
{
    memset(_disk_infos, 0, MAX_PARTITIONS* sizeof(DiskInfo));
}

FileSystemTask::~FileSystemTask()
{

}


void FileSystemTask::run() {
    this->prase_partition_info(0, nullptr);
    uint32_t disk_index = 0;
    for (int i = 0; i < empty_index; ++i) {
        PartitionInfo* p_info = get_partition_info(i);
        if (p_info != nullptr && !p_info->empty) {
            printf("Partion info : \n");
            printf("SystemID = %d | start sector = %d | total sector = %d \n", p_info->system_id,
                   p_info->getAbsStartSector(), p_info->total_sectors);
        }
        if (p_info != nullptr && !p_info->empty && p_info->system_id != PART_WITH_SUB) {
            // get one Partition Info
            DiskInfo* p_disk = &_disk_infos[disk_index];
            p_disk->abs_start_lba = p_info->getAbsStartSector();
            p_disk->total_sectors = p_info->total_sectors;
            // TODO check SystemID and read different FileSystem
            p_disk->type = FAT32;
            disk_index++;
        }
    }

//    for (int j = 0; j < MAX_PARTITIONS; ++j) {
//        DiskInfo* p_disk = &_disk_infos[j];
//        if (p_disk->type != UNKNOW) {
//            printf("Disk info : \n");
//            printf("FileSystem = %d | start sector = %d | total sector = %d \n", p_disk->type, p_disk->abs_start_lba, p_disk->total_sectors);
//        }
//    }

    while (1) {

    }
}

PartitionInfo* FileSystemTask::get_empty_partition_info() {
    PartitionInfo* p = get_partition_info(empty_index);
    p->_parent_partition = nullptr;
    p->start_sectors = 0;
    return p;
}

PartitionInfo* FileSystemTask::get_partition_info(uint32_t index) {
    assert(index < MAX_PARTITIONS);
    return _partition_infos + index;
}

void FileSystemTask::prase_partition_info(uint32_t start_lba, PartitionInfo* parent) {
    /* first read the MBR sector and get the root Partition Table */
    uint8_t* buf = (uint8_t *) kmalloc(SECTOR_SIZE);
    _msg.type = HD_READ;
    _msg.START_LBA = start_lba; // first Partition Table always in Sector 0
    _msg.SECTORS = 1;
    _msg.BUF = buf;
    send_recv(BOTH, HD_DEST, &_msg);

    uint32_t ret = parse_one_sector_partition_info(buf, parent);

    kfree(buf);
}

/**
 * get partition info from one given sector
 * @param buf
 * @return
 */
uint32_t FileSystemTask::parse_one_sector_partition_info(uint8_t *buf, PartitionInfo* parent) {
    uint32_t ret = 0;
    PartitionInfo* p = nullptr;
    /**
     * Get first partition
     */

    p = parse_partition_info_by_index(buf, 0, parent);
    if (p != nullptr && p->system_id == 0x05) {
        prase_partition_info(p->getAbsStartSector(), p);
    }
    if (p) ret++;
    p = parse_partition_info_by_index(buf, 1, parent);
    if (p != nullptr && p->system_id == 0x05) {
        prase_partition_info(p->getAbsStartSector(), p);
    }
    if (p) ret++;
    p = parse_partition_info_by_index(buf, 2, parent);
    if (p != nullptr && p->system_id == 0x05) {
        prase_partition_info(p->getAbsStartSector(), p);
    }
    if (p) ret++;
    p = parse_partition_info_by_index(buf, 3, parent);
    if (p != nullptr && p->system_id == 0x05) {
        prase_partition_info(p->getAbsStartSector(), p);
    }
    if (p) ret++;
    return ret;
}

/**
 * get Partition info of given index
 * @param buf
 * @param index
 * @return
 */
PartitionInfo* FileSystemTask::parse_partition_info_by_index(uint8_t *buf, uint32_t index, PartitionInfo* parent) {
    uint8_t* p_buffer = nullptr;
    switch (index) {
        case 0:
            p_buffer = buf + PARTITION_ONE_OFFSET;
            break;
        case 1:
            p_buffer = buf + PARTITION_TWO_OFFSET;
            break;
        case 2:
            p_buffer = buf + PARTITION_THREE_OFFSET;
            break;
        case 3:
            p_buffer = buf + PARTITION_FOUR_OFFSET;
            break;
        default:
            return nullptr;
    }
    uint8_t system_id = *(p_buffer + SYSTEM_ID_OFFSET);
    if (system_id == 0) { // no partition info
        return nullptr;
    }
    PartitionInfo* p_info = get_empty_partition_info();
    empty_index++;
    p_info->system_id = system_id;
    uint8_t bootinfo = *(p_buffer + BOOT_INFO_OFFSET);
    p_info->boot_indicate = bootinfo;
    uint32_t* start_sector = (uint32_t*)(p_buffer + START_SECTOR_OFFSET);
    p_info->start_sectors = *start_sector;
    uint32_t* total_sector = (uint32_t*)(p_buffer + TOTAL_SECTOR_OFFSET);
    p_info->total_sectors = *total_sector;
    p_info->empty = false;
    p_info->_parent_partition = parent;

    return p_info;
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
