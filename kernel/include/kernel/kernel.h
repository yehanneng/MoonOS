#ifndef MOONOS_KERNEL_H
#define MOONOS_KERNEL_H
#include <kernel/gdt.h>
#include <kernel/tty.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#define KPRINT_BUF _kernel_print_buf
#define KPRINT_BUF_SIZE 255

#define KPRINT_BUF_INIT() \
    char KPRINT_BUF[KPRINT_BUF_SIZE]
/* kernel printf */
#define KPRINTF(format, args...) \
    memset(KPRINT_BUF, 0 ,KPRINT_BUF_SIZE);sprintf(KPRINT_BUF, format, ##args);terminal_write(KPRINT_BUF, strlen(KPRINT_BUF))


#define KERNEL_BASE 0x10000
#define KERNEL_SIZE 0x10000

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
void kernel_info_task_from_interrupt(int pid);
struct _s_proc;

struct _s_proc* kernel_getFocusProcess();

int kernel_proc2pid(struct _s_proc* proc);
int kernel_ldt_seg_linear(struct _s_proc* p, int idx);
struct _s_proc* kernel_pid2proc(int pid);
void* kernel_va2la(int pid, void* va);

/* message function */
struct _message;
int kernel_sendrec(int function, int src_dest, struct _message* msg, struct _s_proc* proc);

/* sys call */
void* kernel_sys_call(int sys_call_vector, void* arg1, void* arg2, void* arg3, struct _s_proc* caller);
void sys_call();


/* fscopy */
#define PHYS_COPY(dst_pid,p_dst, src_pid, p_src, len) \
                memcpy(kernel_va2la(dst_pid,(void*)p_dst), \
                        kernel_va2la(src_pid,(void*)p_src), \
                        len)

#ifdef __cplusplus
}
#endif

#endif // MOONOS_KERNEL_H
