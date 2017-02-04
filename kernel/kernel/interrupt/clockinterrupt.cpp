#include "clockinterrupt.h"

#include <kernel/idt.h>
#include <stdio.h>

extern int k_reenter;

ClockInterruptHandler::ClockInterruptHandler()
:AbstractInterruptHandler(CLOCK_IRQ),_tick(0)
{

}

ClockInterruptHandler::~ClockInterruptHandler()
{

}
/**
 * system clock function
*/
void ClockInterruptHandler::handle_irq()
{
    _tick++;
    if (k_reenter != 0) {
        return;
    }
}
