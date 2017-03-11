//
// Created by Tangruiwen on 2017/3/10.
//

#ifndef MOONOS_FAT32_H
#define MOONOS_FAT32_H

struct _fat32{
    char BS_jmpBoot[3];
    char BS_OEMName[8];
    unsigned short BPB_BytsPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_ResvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short BPB_RootEntCnt;
    unsigned short BPB_TotSec16;
    char BPB_Media;
    unsigned short BPB_FATSz16;
    unsigned short BPB_SecPerTrk;
    unsigned short NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    unsigned int BPB_FATSz32;
    unsigned short BPB_flags;
    unsigned short BPB_FSVer;
    unsigned int BPB_RootClus;
    unsigned short BPB_FSInfo;
    unsigned short BPB_BkBootSec;
    char BPB_Reserved[12];
    unsigned char BS_DrvNum;
    unsigned char BS_Reserved1;
    char BS_BootSig;
    unsigned int BS_VolID;
    char BS_VolLAB[11];
    char BS_FilSysType[8];
    char code[420];
    char bootsig[2];
} __attribute__((packed));

typedef struct _fat32 FAT32_ENTRY;

#define MSDOS_NAME 11

struct _dir_entry{
    char name[8];
    char ext[3];
    uint8_t attrib;
    uint8_t userattrib;

    char undelete;
    uint16_t createtime;
    uint16_t createdate;
    uint16_t accessdate;
    uint16_t clusterhigh;

    uint16_t modifiedtime;
    uint16_t modifieddate;
    uint16_t clusterlow;
    uint32_t filesize;
}__attribute__((packed));

typedef struct _dir_entry DIR_ENTRY;

struct _dir_long_entry {
    unsigned char    id;             /* sequence number for slot */
    unsigned char    name0_4[10];    /* first 5 characters in name */
    unsigned char    attr;           /* attribute byte */
    unsigned char    reserved;       /* always 0 */
    unsigned char    alias_checksum; /* checksum for 8.3 alias */
    unsigned char    name5_10[12];   /* 6 more characters in name */
    unsigned short   start;         /* starting cluster number, 0 in long slots */
    unsigned char    name11_12[4];   /* last 2 characters in name */
} __attribute__((packed));

typedef struct _dir_long_entry DIR_LONG_ENTRY;

struct _inode {
    unsigned int cluster;
    unsigned int cur_clus;
    unsigned int size;
    unsigned int pos;
} __attribute__((packed));

typedef struct _inode INODE;

struct _msdos_sb {
    unsigned int sec_per_clus;
    unsigned int root_sec;
    unsigned int first_fat_sec;
    unsigned int first_data_sec;
    unsigned int cur_dir_clus;
} __attribute__((packed));

typedef struct _msdos_sb MSDOS_SB;


#endif //MOONOS_FAT32_H
