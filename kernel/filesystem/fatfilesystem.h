//
// Created by Tangruiwen on 2017/3/10.
//

#ifndef MOONOS_FATFILESYSTEM_H
#define MOONOS_FATFILESYSTEM_H

#include <stdint.h>
#include "fat32.h"

class FATFileSystem {
public:
    FATFileSystem(uint32_t start_lba);
    virtual ~FATFileSystem();
    uint32_t init(uint8_t* buf);
    inline uint32_t getStartLBA() {return this->start_lba;}
    uint32_t getRootSec();
protected:
    uint32_t start_lba;
    uint32_t cluster_size;

private:
    FAT32_ENTRY mFat32;
    MSDOS_SB msdos_sb;
    uint32_t mFATSz;
};


#endif //MOONOS_FATFILESYSTEM_H
