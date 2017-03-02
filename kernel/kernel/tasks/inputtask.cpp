//
// Created by Tangruiwen on 2017/2/25.
//

#include <kernel/kernel.h>
#include <kernel/keyboard.h>
#include <kernel/idt.h>
#include <kernel/proc.h>
#include <stdio.h>
#include "InputTask.h"

InputTask::InputTask()
:AbstractTask::AbstractTask()
{

}

InputTask::~InputTask() {

}

void InputTask::run()
{
    while(1) {
        this->do_input_read();
        this->do_input_write();
    }
}

void InputTask::do_input_read()
{
    KeyboardInterruptHandler* kernel_keyboard_handler = (KeyboardInterruptHandler *) get_irq_handler(KEYBOARD_IRQ);
    uint8_t key = kernel_keyboard_handler->keyboard_read();
    if (key != 0) {
        printf("i");
        PROCESS* proc = kernel_getFocusProcess();
        if (proc->_input_buffer.count < TTY_IN_BYTES) {
            *(proc->_input_buffer._p_head) = key;
            proc->_input_buffer._p_head++;
            if (proc->_input_buffer._p_head == proc->_input_buffer._buff + TTY_IN_BYTES) {
                proc->_input_buffer._p_head = proc->_input_buffer._buff;
            }
        }
    }
}

void InputTask::do_input_write() {

}


extern "C" void input_task_main()
{
    InputTask inputTask;
    inputTask.run();
}