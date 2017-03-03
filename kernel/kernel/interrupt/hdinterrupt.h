//
// Created by Tangruiwen on 2017/3/3.
//

#ifndef MOONOS_HDINTERRUPT_H
#define MOONOS_HDINTERRUPT_H

#include <kernel/interrupt/interrupt.h>
#include <kernel/idt.h>

class HDInterruptHandler: public AbstractInterruptHandler{
public:
    HDInterruptHandler();
    virtual ~HDInterruptHandler();
    virtual void handle_irq() override ;
};

#endif //MOONOS_HDINTERRUPT_H
