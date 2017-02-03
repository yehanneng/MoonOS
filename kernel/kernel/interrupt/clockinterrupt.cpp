#include "clockinterrupt.h"

#include <kernel/idt.h>
#include <stdio.h>

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
    printf("clock handler");
}
