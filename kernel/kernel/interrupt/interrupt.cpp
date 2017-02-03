#include <kernel/interrupt/interrupt.h>
#include <liballoc.h>

AbstractInterruptHandler::AbstractInterruptHandler(uint8_t vector)
{
    this->self_vector = vector;
}

AbstractInterruptHandler::~AbstractInterruptHandler()
{

}

AbstractInterruptHandler* irq_table[NR_IRQ];

extern "C" void spurious_irq(int irq)
{
    if(irq < NR_IRQ){
        irq_table[irq]->handle_irq();
    }
}

extern "C" void __cxa_pure_virtual()
{

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

extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
    arg;
    dso;
    return 0;
}

extern "C" void __cxa_finalize(void *f)
{
    f;
}
