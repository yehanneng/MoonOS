#ifndef MOONOS_KERNEL_H
#define MOONOS_KERNEL_H
#include <kernel/gdt.h>

/************* asm function*****************/
void set_gdt(const char* gdtr);

/******* kernel.c ********/
void kernel_init_gdt();

#endif // MOONOS_KERNEL_H
