//
// Created by Tangruiwen on 2017/3/10.
//

#ifndef MOONOS_FATFILESYSTEM_H
#define MOONOS_FATFILESYSTEM_H

#include <stdint.h>
#include "fat32.h"

#define CACHE_ENTRY_SIZE 16
#define RADIX_THRESHOLD 4
#define RADIX_SIZE_LV2 CACHE_ENTRY

#ifdef __cplusplus

class FATFileSystem {
public:
    FATFileSystem(uint32_t start_lba);
    virtual ~FATFileSystem();
    uint32_t init(uint8_t* buf);
    inline uint32_t getStartLBA() {return this->start_lba;}
    uint32_t getFirstDataSector();
    uint32_t getFirstFatSector();
    void listRootContent(uint8_t* buf);
    DIR_ENTRY* openFile(uint8_t* rootDirBuf, const char* filename, uint32_t nameLength);

private:
    ADDRESS_SPACE* alloc_address_space();
    ADDRESS_SPACE* bread_sector(uint32_t sector);
protected:
    uint32_t start_lba;
    uint32_t cluster_size;

private:
    FAT32_ENTRY mFat32;
    MSDOS_SB msdos_sb;
    uint32_t mFATSz;
    uint32_t address_index;
    ADDRESS_SPACE address_buff[CACHE_ENTRY_SIZE];
};

#endif
#endif //MOONOS_FATFILESYSTEM_H
