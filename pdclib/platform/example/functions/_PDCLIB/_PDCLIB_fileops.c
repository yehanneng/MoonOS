/* _PDCLIB_fileops

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef REGTEST
#include <stdio.h>
#include <stdint.h>
#include "_PDCLIB_glue.h"
#include <errno.h>
#include <message.h>

static bool readf( _PDCLIB_fd_t self, void * buf, size_t length, 
                   size_t * numBytesRead )
{
    MESSAGE _msg;
    _msg.type = DEV_READ;
    _msg.FD = self.sval;
    _msg.BUF = buf;
    _msg.CNT = length - 1;
    int ret = send_recv(BOTH, FS_DEST, &_msg);
    if (ret != 0 || _msg.CNT == 0) {
        errno = ENOTSUP;
        return false;
    }else {
        int recRead = (size_t)_msg.CNT;
        *numBytesRead = recRead;
        return true;
    }
}

static bool writef( _PDCLIB_fd_t self, const void * buf, size_t length, 
                   size_t * numBytesWritten )
{
    MESSAGE _msg;
    _msg.type = DEV_WRITE;
    _msg.FD = self.uval;
    _msg.BUF = buf;
    _msg.CNT = length;
    int ret = send_recv(BOTH, FS_DEST, &_msg);
    int retv = _msg.RETVAL;
    if (ret != 0) {
        errno = ENOTSUP;
        return false;
    }else {
        *numBytesWritten = (size_t)retv;
        return true;
    }
}
static bool seekf( _PDCLIB_fd_t self, int_fast64_t offset, int whence,
    int_fast64_t* newPos )
{
    printf("seekf\n");
    errno = ENOTSUP;
    return false;
}

static void closef( _PDCLIB_fd_t self )
{
    printf("close\n");
    errno = ENOTSUP;
}

const _PDCLIB_fileops_t _PDCLIB_fileops = {
    .read  = readf,
    .write = writef,
    .seek  = seekf,
    .close = closef,
};

#endif

#ifdef TEST
#include "_PDCLIB_test.h"

int main( void )
{
    // Tested by stdio test cases
    return TEST_RESULTS;
}

#endif
