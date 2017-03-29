//
// Created by Tangruiwen on 2017/3/29.
//

#ifndef MOONOS_FILEDESCRIPTOR_H
#define MOONOS_FILEDESCRIPTOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileDescriptor{
    int mode;
    int cursor;
    int available;
    void* data;
} FileDescriptor;

typedef struct FileP {
    int available;
    FileDescriptor* p_file_desc;
}FileP;

#define FILE_INDEX_OFFSET 3

#ifdef __cplusplus
}
#endif

#endif //MOONOS_FILEDESCRIPTOR_H
