//
// Created by Tangruiwen on 2017/3/10.
//

#include <string.h>
#include <stdio.h>
#include <kernel/kernel.h>
#include <liballoc.h>
#include "fatfilesystem.h"
#include "../kernel/tasks/harddisktask.h"

/*
 * printf is handle by FS process so in FS process can not use printf
 * instead should KPRINTF macro
 * */
KPRINT_BUF_INIT();

FATFileSystem::FATFileSystem(uint32_t start_lba)
:start_lba(start_lba),cluster_size(0),mFATSz(0),address_index(0)
{
    memset(&msdos_sb,0, sizeof(MSDOS_SB));
}

FATFileSystem::~FATFileSystem() {

}

uint32_t FATFileSystem::init(uint8_t* buf) {
    if (buf == nullptr) return 0;
    memcpy(&mFat32, buf, sizeof(FAT32_ENTRY));
    if (mFat32.BPB_FATSz16) {
        mFATSz = mFat32.BPB_FATSz16;
    } else if (mFat32.BPB_FATSz32) {
        mFATSz = mFat32.BPB_FATSz32;
    }

    /* init fat disk layout */
    msdos_sb.root_sec = start_lba + ((mFat32.BPB_RootEntCnt*32)+(mFat32.BPB_BytsPerSec-1))/mFat32.BPB_BytsPerSec;
    msdos_sb.first_fat_sec = start_lba + mFat32.BPB_ResvdSecCnt;
    msdos_sb.first_data_sec = mFat32.BPB_ResvdSecCnt+(mFat32.BPB_NumFATs * mFATSz) + msdos_sb.root_sec;
    msdos_sb.sec_per_clus = mFat32.BPB_SecPerClus;
    msdos_sb.cur_dir_clus = 2;
    cluster_size = msdos_sb.sec_per_clus * SECTOR_SIZE;
    return 1;
}

uint32_t FATFileSystem::getFirstDataSector() {
    return msdos_sb.first_data_sec;
}

uint32_t FATFileSystem::getFirstFatSector() {
    return msdos_sb.first_fat_sec;
}

void FATFileSystem::listRootContent(uint8_t *buf) {
    for (uint32_t i = 0; i < (SECTOR_SIZE / sizeof(DIR_ENTRY)); ++i) {
        DIR_ENTRY* p_dir = (DIR_ENTRY*)(buf + i * sizeof(DIR_ENTRY));
        uint8_t first_byte = *((uint8_t*)p_dir);
        if (first_byte == 0xE5){
            // skip this entry
        } else if(first_byte == 0){ // this is the last entry
            KPRINTF("break loop in %d\n", i);
            break;
        } else {
            if (p_dir->attrib != 0x0f) {
                uint8_t name[10];
                memcpy(name, p_dir->name, 8);
                name[8] = 0;
                KPRINTF("name = %s | attr = %x | first data cluster = %d | size = %d\n", name, p_dir->attrib,
                       (p_dir->clusterhigh << 16 | p_dir->clusterlow), p_dir->filesize);
            } else { // this is a long dir entry
                DIR_LONG_ENTRY *p_long_dir = (DIR_LONG_ENTRY *) p_dir;
                KPRINTF("id = %x | attr = %x\n", p_long_dir->id, p_long_dir->attr);
            }
        }
    }
}

void* FATFileSystem::openFile(uint8_t* rootDirBuf, const char *filename, uint32_t nameLength) {

    for (uint32_t i = 0; i < (SECTOR_SIZE / sizeof(DIR_ENTRY)); ++i) {
        DIR_ENTRY* p_dir = (DIR_ENTRY*)(rootDirBuf + i * sizeof(DIR_ENTRY));
        uint8_t first_byte = *((uint8_t*)p_dir);
        if (first_byte == 0xE5){
            // skip this entry
        } else if(first_byte == 0){ // this is the last entry
            break;
        } else {
            if (p_dir->attrib != 0x0f) {
                uint8_t name[10] = {0};
                memcpy(name, p_dir->name, 8);
                name[8] = 0;
                int ret = strcmp(filename, (const char*)name);
                if (ret == 0) {
                    DIR_ENTRY* p = (DIR_ENTRY*)kmalloc(sizeof(DIR_ENTRY));
                    memcpy(p, p_dir, sizeof(DIR_ENTRY));
                    return p;
                }
            } else { // this is a long dir entry
                // DIR_LONG_ENTRY *p_long_dir = (DIR_LONG_ENTRY *) p_dir;
            }
        }
    }
    return nullptr;
}

int FATFileSystem::getFirstFileDataSector(FileDescriptor *p_descriptor) {
    DIR_ENTRY* p_dir = (DIR_ENTRY *) p_descriptor->data;

    return (p_dir->clusterhigh << 16 | p_dir->clusterlow) * this->msdos_sb.sec_per_clus - 4 + this->msdos_sb.first_data_sec;
}

int FATFileSystem::getFileSize(FileDescriptor *p_descripor) {
    DIR_ENTRY* p_dir = (DIR_ENTRY *) p_descripor->data;
    return p_dir->filesize;
}

ADDRESS_SPACE* FATFileSystem::alloc_address_space() {
    if(this->address_index < CACHE_ENTRY_SIZE) {
        ADDRESS_SPACE* p = this->address_buff + this->address_index;
        memset(p,0, sizeof(ADDRESS_SPACE));
        return p;
    } else {
        return nullptr;
    }
}

void* FATFileSystem::createFile(const char *fileName, uint32_t nameLength) {

    return nullptr;
}

int FATFileSystem::readFromFile(uint8_t *buf, uint32_t bufLength, FileDescriptor *fileDescriptor) {

    return 0;
}

ADDRESS_SPACE* FATFileSystem::bread_sector(uint32_t sector) {
    ADDRESS_SPACE* addr = nullptr;


    return addr;
}

bool FATFileSystem::fileNameMatch(FileDescriptor *fileDescriptor, const char *fileName, uint32_t nameLength) {
    if (nameLength > 8) {
        return false;
    }
    if (fileDescriptor->available) {
        DIR_ENTRY* p_dir_engry = (DIR_ENTRY *) fileDescriptor->data;
        if(strcmp(fileName, p_dir_engry->name) == 0) {
            return true;
        }
    }
    return false;
}