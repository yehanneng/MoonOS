//
// Created by Tangruiwen on 2017/3/4.
//

#include "ticktask.h"
#include <kernel/idt.h>
#include <stdlib.h>
#include <kernel/interrupt/interrupt.h>
#include <stdio.h>
#include <message.h>
#include <kernel/memory.h>
#include "../interrupt/clockinterrupt.h"

TickTask::TickTask() {
    _clockHandler = (ClockInterruptHandler *) get_irq_handler(CLOCK_IRQ);
}

TickTask::~TickTask() {

}

void TickTask::run() {
    MESSAGE _msg;
    int ret = 0;
    while (1) {
        ret = send_recv(RECEIVE, ANY, &_msg);
        if (ret == 0 && _msg.type == GET_TICKS) {
            _msg.RETVAL = _clockHandler->getTick();
            send_recv(SEND, _msg.source, &_msg);
        } else if(_msg.type == SYS_ALLOC_PAGE){
            int pageNum = _msg.PAGE_NUM;
            void* memaddr = kalloc_page(pageNum);
            _msg.RETVAL =(int) memaddr;
            int src = _msg.source;
            send_recv(SEND, src, &_msg);
        } else if (_msg.type == SYS_FREE_PAGE) {
            int pageNum = _msg.PAGE_NUM;
            void* p = _msg.PAGEADDR;
            kfree_page(p, pageNum);
            _msg.RETVAL =0;
            int src = _msg.source;
            send_recv(SEND, src, &_msg);
        }
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void tick_task_main() {
    // printf("tick_task_main\n");
    TickTask _tick_task;
    _tick_task.run();
}
#ifdef __cplusplus
}
#endif

