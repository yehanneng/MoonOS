//
// Created by Tangruiwen on 2017/3/8.
//

#ifndef MOONOS_FILESYSTEMTASK_H
#define MOONOS_FILESYSTEMTASK_H

#include <kernel/kernel.h>
#include <kernel/tasks/task.h>
#include <absfilesystem.h>
#include <filedescriptor.h>
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

#define FILE_DESC_BUFFER_SIZE 64

#define MAX_PARTITIONS 16
#define HAS_BOOT_CODE 0x80
#define NO_BOOT_CODE 0x00

#define PART_WITH_SUB 0x05


#ifdef __cplusplus

class PartitionInfo{
public:
    PartitionInfo();
    virtual ~PartitionInfo();
    uint32_t getAbsStartSector();
public:
    bool empty;
    uint8_t boot_indicate;
    uint8_t system_id;
    uint32_t base_sectors;
    uint32_t start_sectors;
    uint32_t total_sectors;
    PartitionInfo* _parent_partition;
};

enum FileSystem{
    UNKNOW = 0,
    FAT32,
    EXTFS
} ;

struct _disk_info{
    uint32_t abs_start_lba;
    uint32_t total_sectors;
    FileSystem type;
    _disk_info():abs_start_lba(0), total_sectors(0), type(UNKNOW){}
};

typedef struct _disk_info DiskInfo;

class FileSystemTask: public AbstractTask {
public:
    FileSystemTask();
    virtual ~FileSystemTask();
    virtual void run() override ;

private:
    void prase_partition_info(uint32_t start_lba, PartitionInfo* parent);
    PartitionInfo* get_empty_partition_info();
    PartitionInfo* get_partition_info(uint32_t index);
    uint32_t parse_one_sector_partition_info(uint8_t* buf, PartitionInfo* parent);
    PartitionInfo* parse_partition_info_by_index(uint8_t* buf, uint32_t index, PartitionInfo* parent);
    uint32_t read_disk_by_message(uint8_t* buf, uint32_t start_sec, uint32_t secs_to_read);
    uint32_t do_file_open(int caller, uint8_t* rootDirBuf, const char* pathName, uint32_t nameLength);
private:
    uint32_t empty_index;
    PartitionInfo _partition_infos[MAX_PARTITIONS];
    DiskInfo _disk_infos[MAX_PARTITIONS];
    FileDescriptor mFileDescriptors[FILE_DESC_BUFFER_SIZE];
    MESSAGE _msg;
    AbsFileSystem* mFileSystem;
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
