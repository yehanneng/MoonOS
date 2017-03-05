//
// Created by Tangruiwen on 2017/3/5.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int get_ticket()
{
    MESSAGE _msg;
    int ticket = -1;
    int ret = 0;
    _msg.type = GET_TICKS;
    ret = send_recv(BOTH, 2, &_msg);
    if(ret == 0) {
        ticket = _msg.RETVAL;
    }

    return ticket;
}