#ifndef MOONOS_INTERRUPT_H
#define MOONOS_INTERRUPT_H
#include <stdint.h>

#ifdef __cplusplus

#define NR_IRQ 16

class AbstractInterruptHandler{
public:
    AbstractInterruptHandler(uint8_t vector);
    virtual ~AbstractInterruptHandler();
    virtual void handle_irq(void) = 0;
    inline uint8_t getVectorNum() { return this->self_vector; }
protected:
    uint8_t self_vector;

};

#endif




#ifdef __cplusplus
extern "C" {
#endif


void spurious_irq(int irq);
void init_irq_table();

#ifdef __cplusplus
}
#endif

#endif // MOONOS_INTERRUPT_H
