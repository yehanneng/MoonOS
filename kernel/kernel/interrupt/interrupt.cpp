#include <kernel/interrupt/interrupt.h>
#include <kernel/idt.h>
#include <liballoc.h>
#include <stdio.h>

#include "clockinterrupt.h"

AbstractInterruptHandler::AbstractInterruptHandler(uint8_t vector)
{
    this->self_vector = vector;
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

extern "C" void spurious_irq(int irq)
{
    cxx_irq_router(irq);
}

extern "C" void init_irq_table()
{
    irq_table[CLOCK_IRQ] = new ClockInterruptHandler();
}

void operator delete(void * p) // or delete(void *, std::size_t)
{
    kfree(p);
}

void operator delete[](void* p)
{
    kfree(p);
}

void* operator new(size_t size)
{
    return kmalloc(size);
}

void* operator new[](size_t size)
{
    return kmalloc(size);
}
