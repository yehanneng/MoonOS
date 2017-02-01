#include <kernel/interrupt/interrupt.h>

AbstractInterruptHandler* irq_table[NR_IRQ];

extern "C" void spurious_irq(int irq)
{
    if(irq < NR_IRQ){
        irq_table[irq]->handle_irq();
    }
}
