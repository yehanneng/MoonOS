//
// Created by Tangruiwen on 2017/3/10.
//

#include <string.h>
#include <stdio.h>
#include "fatfilesystem.h"
#include "../kernel/tasks/harddisktask.h"

FATFileSystem::FATFileSystem(uint32_t start_lba)
:start_lba(start_lba),mFATSz(0),cluster_size(0)
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
    printf("mFATSz = %d | root sector = %d\n", mFATSz, msdos_sb.root_sec);
    printf("first fat sec = %d | first data sec = %d\n", msdos_sb.first_fat_sec, msdos_sb.first_data_sec);
    printf("sector per clus = %d | cluster size = %d\n", msdos_sb.sec_per_clus, cluster_size);
    return 1;
}

uint32_t FATFileSystem::getFirstDataSector() {
    return msdos_sb.first_data_sec;
}

uint32_t FATFileSystem::getFirstFatSector() {
    return msdos_sb.first_fat_sec;
}
