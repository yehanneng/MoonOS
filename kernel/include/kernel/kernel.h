#ifndef MOONOS_KERNEL_H
#define MOONOS_KERNEL_H
#include <kernel/gdt.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/************* asm function*****************/
void restart();

void set_gdt(const char* gdtr);
void set_cr3(unsigned int page_derectory_addr);
void out_byte(unsigned short port,unsigned char value);
uint8_t in_byte(uint16_t port);
uint16_t port_read(uint16_t port);
void load_idt(const char* idtr);
void load_tss(unsigned int tss_selector);
/******* kernel.c ********/
void kernel_init_gdt();
void kernel_init_idt();
void kernel_schedule_process();
void kernel_init_internal_process();

struct _s_proc;

struct _s_proc* kernel_getFocusProcess();
#ifdef __cplusplus
}
#endif

#endif // MOONOS_KERNEL_H
