/* _PDCLIB_allocpages( int const )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

/* This is a stub implementation of _PDCLIB_allocpages
*/

#include <stdint.h>
#include <stddef.h>
#include "_PDCLIB_glue.h"
#include <errno.h>
#include <message.h>

void * _PDCLIB_allocpages( size_t n )
{
    MESSAGE _msg;
    _msg.type = SYS_ALLOC_PAGE;
    _msg.PAGE_NUM = n;
    int ret = send_recv(BOTH, TICK_DEST, &_msg);
    if (ret != 0) {
        errno = ENOTSUP;
        return NULL;
    }else {
        return (void*)_msg.RETVAL;
    }
}

#ifdef TEST
#include "_PDCLIB_test.h"

int main( void )
{
    return TEST_RESULTS;
}

#endif
