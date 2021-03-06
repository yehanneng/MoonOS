//
// Created by Tangruiwen on 2017/3/8.
//

#include "harddisktask.h"
#include "filesystemtask.h"
#include <fatfilesystem.h>
#include <assert.h>
#include <stdio.h>
#include <liballoc.h>
#include <kernel/proc.h>
#include <kernel/kernel.h>
#include <string.h>

#define MIN(a, b) a < b ? a : b

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
    for (int i = 0; i < FILE_DESC_BUFFER_SIZE; ++i) {
        mFileDescriptors[i].available = -1;
    }
}

FileSystemTask::~FileSystemTask()
{

}


void FileSystemTask::run() {
    this->prase_partition_info(0, nullptr);
    uint32_t disk_index = 0;
    if (this->empty_index != 0) {
        for (uint32_t i = 0; i < empty_index; i++) {
            PartitionInfo *p_info = get_partition_info(i);
            if (p_info != nullptr && !p_info->empty && p_info->system_id != PART_WITH_SUB) {
                // get one Partition Info
                DiskInfo *p_disk = &_disk_infos[disk_index];
                p_disk->abs_start_lba = p_info->getAbsStartSector();
                p_disk->total_sectors = p_info->total_sectors;
                // TODO check SystemID and read different FileSystem
                p_disk->type = FAT32;
                disk_index++;
            }
        }
        DiskInfo *p_disk = &_disk_infos[0];
        uint8_t _buf[SECTOR_SIZE];
        if (p_disk->type == FAT32) { // support other file system
            mFileSystem = new FATFileSystem(p_disk->abs_start_lba);
        }
        uint32_t ret = read_disk_by_message(_buf, mFileSystem->getStartLBA(), 1);
        if (ret == 0) {
            mFileSystem->init(_buf);
            ret = read_disk_by_message(_buf, mFileSystem->getFirstDataSector(), 1);
            if (ret == 0) {
                // success parse root directory
            }
        }
    }
    while (1) {
        uint32_t ret = send_recv(RECEIVE, ANY, &_msg);
        if (ret == 0) {
            if (_msg.type == DEV_WRITE) {
                if (_msg.FD == 1) {
                    int len = _msg.CNT;
                    int src = _msg.source;
                    terminal_write((const char *) _msg.BUF, len);
                    _msg.type = DEV_WRITE;
                    _msg.RETVAL = len;
                    send_recv(SEND, src, &_msg);
                }
            } else if (_msg.type == DEV_OPEN) {
                char pathname[MAX_NAME_LEN];
                memset(pathname, 0, MAX_NAME_LEN);
                uint32_t nameLen = _msg.NAME_LEN;
                PHYS_COPY(FS_DEST, pathname, _msg.source, _msg.PATHNAME, nameLen);

                int src = _msg.source;
                int fdIndex = -1;
                if (this->check_fileDesc_cache(pathname, strlen(pathname), &fdIndex)) { // find file cache in memory

                }

                uint8_t _buf[SECTOR_SIZE];
                uint32_t ret = read_disk_by_message(_buf, mFileSystem->getFirstDataSector(), 1);
                if (ret == 0)
                    _msg.RETVAL = do_file_open(src, _buf, pathname, nameLen);
                else
                    _msg.RETVAL = -1;
                send_recv(SEND, src, &_msg);
            } else if (_msg.type == DEV_READ) {
                int src = _msg.source;
                int fd = _msg.FD;
                void* readBuf = (void*) kernel_va2la(src, _msg.BUF);
                int lengthToRead = _msg.CNT;

                uint8_t kbuf[SECTOR_SIZE] = {0};
                PROCESS* p_caller = kernel_pid2proc(src);
                FileP* p_file = &p_caller->filp[fd - FILE_INDEX_OFFSET];

                assert(p_file->available);
                int dataSector = mFileSystem->getFirstFileDataSector(p_file->p_file_desc);
                int ret = read_disk_by_message(kbuf,dataSector, 1);
                if (ret == 0) {
                    int fileSize = mFileSystem->getFileSize(p_file->p_file_desc);
                    int recLen = MIN(fileSize + 1, lengthToRead);
                    if (recLen >= SECTOR_SIZE) recLen = SECTOR_SIZE - 1;
                    kbuf[recLen] = -1;
                    memcpy(readBuf, kbuf, fileSize < lengthToRead ? fileSize : lengthToRead);
                    _msg.CNT = recLen;
                } else {
                    _msg.CNT = 0;
                }

                send_recv(SEND, src, &_msg);
            }
        }
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

    uint32_t ret = 0;
    ret = read_disk_by_message(buf, start_lba, 1);
    if (ret == 0) {
        ret = parse_one_sector_partition_info(buf, parent);
    }
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


uint32_t FileSystemTask::read_disk_by_message(uint8_t *buf, uint32_t start_sec, uint32_t secs_to_read) {
    uint32_t  ret = 0;
    _msg.type = HD_READ;
    _msg.START_LBA = start_sec; // first Partition Table always in Sector 0
    _msg.SECTORS = secs_to_read;
    _msg.BUF = buf;
    ret = send_recv(BOTH, HD_DEST, &_msg);

    return ret;
}

uint32_t FileSystemTask::do_file_open(int caller,uint8_t* rootDirBuf,const char *pathName, uint32_t nameLength) {
    DIR_ENTRY* p_dir = (DIR_ENTRY*)mFileSystem->openFile(rootDirBuf, pathName, nameLength);
    if (p_dir == nullptr) {
        return -1;
    }
    FileDescriptor* p_file_desc = nullptr;
    for (int i = 0; i < FILE_DESC_BUFFER_SIZE; ++i) {
        if (mFileDescriptors[i].available == -1) {
            p_file_desc = &mFileDescriptors[i];
            break;
        }
    }

    if (p_file_desc == nullptr) {
        return -1;
    }
    p_file_desc->data = p_dir;
    int ret = -1;
    PROCESS* p_caller = kernel_pid2proc(caller);
    for (int j = 0; j < NR_OPEN; ++j) {
        if (p_caller->filp[j].available == -1) {
            ret = j;
            p_caller->filp[j].available = 1;
            p_caller->filp[j].p_file_desc = p_file_desc;
            break;
        }
    }
    if (ret >= 0) {
        return ret + FILE_INDEX_OFFSET;
    }

    return -1;
}


bool FileSystemTask::check_fileDesc_cache(const char *pathName, uint32_t nameLength, int* fdIndex) {

    for(int i = 0; i < FILE_DESC_BUFFER_SIZE; i++) {
        FileDescriptor* fP = &mFileDescriptors[i];
        if (fP->available) {
            bool ret = mFileSystem->fileNameMatch(fP, pathName, nameLength);
            if (ret) {
                *fdIndex = i;
                return true;
            }
        }
    }
    return false;
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
