#ifndef MOONOS_CLOCK_H
#define MOONOS_CLOCK_H

#include <kernel/interrupt/interrupt.h>

class ClockInterruptHandler: public AbstractInterruptHandler
{
public:
    ClockInterruptHandler();
    virtual ~ClockInterruptHandler();
    virtual void handle_irq() override;
private:
    unsigned int _tick;
};

#endif // MOONOS_CLOCK_H
