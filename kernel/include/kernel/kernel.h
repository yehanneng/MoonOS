#ifndef MOONOS_KERNEL_H
#define MOONOS_KERNEL_H
#include <kernel/gdt.h>

/************* asm function*****************/
void set_gdt(const char* gdtr);
void set_cr3(unsigned int page_derectory_addr);
void out_byte(unsigned short port,unsigned char value);
void load_idt(const char* idtr);
/******* kernel.c ********/
void kernel_init_gdt();
void kernel_init_idt();

#endif // MOONOS_KERNEL_H
