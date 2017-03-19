#include <kernel/interrupt/interrupt.h>
#include <kernel/idt.h>
#include <liballoc.h>
#include <stdio.h>

#include "clockinterrupt.h"
#include "hdinterrupt.h"
#include <kernel/keyboard.h>

AbstractInterruptHandler::AbstractInterruptHandler(uint8_t vector)
:self_vector(vector)
{

}

AbstractInterruptHandler::~AbstractInterruptHandler()
{

}

AbstractInterruptHandler* irq_table[NR_IRQ];

// ClockInterruptHandler* _clocker = new ClockInterruptHandler();
void cxx_irq_router(int irq)
{
    if(irq < NR_IRQ){
        AbstractInterruptHandler* p = irq_table[irq];
        p->handle_irq();
    }
}

AbstractInterruptHandler* get_irq_handler(uint8_t vector)
{
    if (vector < NR_IRQ) {
        return irq_table[vector];
    } else {
        return NULL;
    }
}

extern "C" void spurious_irq(int irq)
{
    cxx_irq_router(irq);
}

extern "C" void init_irq_table()
{
    irq_table[CLOCK_IRQ] = new ClockInterruptHandler();
    irq_table[KEYBOARD_IRQ] = new KeyboardInterruptHandler();
    irq_table[AT_WINI_IRQ] = new HDInterruptHandler();
}

void operator delete(void * p) // or delete(void *, std::size_t)
{
    kfree(p);
}

void operator delete[](void* p)
{
    kfree(p);
}

void* operator new(unsigned int size)
{
    return kmalloc(size);
}

void* operator new[](unsigned int size)
{
    return kmalloc(size);
}
