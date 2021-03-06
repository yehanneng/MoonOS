/* _PDCLIB_open( char const * const, int )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

/* This is a stub implementation of open.
*/

#include <stdio.h>
#include <errno.h>
#include <message.h>
#include <string.h>

#ifndef REGTEST
#include "_PDCLIB_glue.h"

extern const _PDCLIB_fileops_t _PDCLIB_fileops;

bool _PDCLIB_open( _PDCLIB_fd_t * pFd, const _PDCLIB_fileops_t ** pOps,
                   char const * const filename, unsigned int mode )
{
    MESSAGE _msg;
    _msg.type = DEV_OPEN;
    _msg.PATHNAME = filename;
    _msg.NAME_LEN = strlen(filename);
    int ret = send_recv(BOTH, FS_DEST, &_msg);
    if (ret != 0) {
        errno = ENOTSUP;
        return false;
    } else {
        *pOps =  &_PDCLIB_fileops;
        pFd->sval = _msg.RETVAL;
        return true;
    }
}

#endif

#ifdef TEST
#include "_PDCLIB_test.h"

#include <stdlib.h>
#include <string.h>

int main( void )
{
    return TEST_RESULTS;
}

#endif

