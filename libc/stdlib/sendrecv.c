//
// Created by Tangruiwen on 2017/3/3.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * asm sys_call interface
 */
static int sendrec(int function, int src_dest,const MESSAGE* msg);

int send_recv(int function, int src_dest,const MESSAGE* msg)
{
    int ret = 0;
    if (function == RECEIVE) {
        memset((void*)msg, 0, sizeof(MESSAGE));
    }

    switch (function) {
        case BOTH:
            ret = sendrec(SEND, src_dest, msg);
            if (ret == 0) {
                ret = sendrec(RECEIVE, src_dest, msg);
            }
            break;
        case SEND:
        case RECEIVE:
            ret = sendrec(function, src_dest, msg);
            break;
        default:
            assert((function == SEND)
                    || (function == RECEIVE)
                    || (function == BOTH));
            break;
    }

    return ret;
}