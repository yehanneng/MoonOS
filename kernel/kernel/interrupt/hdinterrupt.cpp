//
// Created by Tangruiwen on 2017/3/3.
//
#include "hdinterrupt.h"
#include <kernel/kernel.h>
#include <stdlib.h>
#include <kernel/tasks/task.h>

HDInterruptHandler::HDInterruptHandler()
:AbstractInterruptHandler(AT_WINI_IRQ)
{

}

HDInterruptHandler::~HDInterruptHandler()
{

}

/**
 * send message to HD Task
 */
void HDInterruptHandler::handle_irq()
{
    kernel_info_task_from_interrupt(TASK_HD);
}