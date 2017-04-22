//
// Created by Administrator on 2017/4/3 0003.
//

#ifndef MOONOS_ABSFILESYSTEM_H
#define MOONOS_ABSFILESYSTEM_H

#include <stdint.h>
#include <filedescriptor.h>
#ifdef __cplusplus

class FileEntry {
public:
    FileEntry() {}
    virtual ~FileEntry(){}

};

class AbsFileSystem {
public:
    AbsFileSystem(){}
    virtual ~AbsFileSystem(){}
    virtual uint32_t getStartLBA() = 0;
    virtual uint32_t getFirstDataSector() = 0;
    virtual uint32_t init(uint8_t* buf) = 0;
    virtual int getFirstFileDataSector(FileDescriptor* p_descriptor) = 0;
    virtual void* openFile(uint8_t* rootDirBuf, const char* filename, uint32_t nameLength) = 0;
    virtual int getFileSize(FileDescriptor* fileDescriptor) = 0;
    virtual void* createFile(const char* fileName, uint32_t nameLength) = 0;
    virtual int readFromFile(uint8_t* buf, uint32_t bufLength, FileDescriptor* fileDescriptor) = 0;
    virtual bool fileNameMatch(FileDescriptor* fileDescriptor, const char* fileName, uint32_t nameLength) = 0;
};

#endif

#endif //MOONOS_ABSFILESYSTEM_H
