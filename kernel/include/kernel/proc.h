#ifndef MOONOS_PROC_H
#define MOONOS_PROC_H
#include <stdint.h>
#include <kernel/gdt.h>

#define NR_TASKS 4
#define NR_PROCS 0

#define TASK_STACK_SIZE 0x8000

#define SENDING   0x02	/* set when proc trying to send */
#define RECEIVING 0x04	/* set when proc trying to recv */
#define WAITING   0x08	/* set when proc waiting for the child to terminate */
#define HANGING   0x10	/* set when proc exits without being waited by parent */
#define FREE_SLOT 0x20	/* set when proc table entry is not used */

/* 每个任务有一个单独的 LDT, 每个 LDT 中的描述符个数: */
#define LDT_SIZE		2

struct _s_stackframe{
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t kernel_esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t retaddr;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

typedef struct _s_stackframe P_FRAME;

#define TTY_IN_BYTES 32



typedef struct{
    uint32_t _buff[TTY_IN_BYTES];
    uint32_t* _p_head;
    uint32_t* _p_tail;
    uint32_t count;
}TTY;

typedef struct {
    uint32_t current_display_addr;
    uint32_t origin_display_addr;
    uint32_t v_mem_limit;
    uint32_t cursor_pos;
}CONSOLE;

struct _message;

struct _s_proc{
    P_FRAME regs;
    uint16_t ldt_sel;
    DESCRIPTOR ldts[LDT_SIZE];
    int ticks;
    int priority;
	int p_flags;
    uint32_t pid;
	struct _message* p_msg;
	int p_recvfrom;
	int p_sendto;
	int has_int_msg;           /**
							   * nonzero if an INTERRUPT occurred when
							   * the task is not ready to deal with it.
							   */
	struct _s_proc * q_sending;   /**
							   * queue of procs sending messages to
							   * this proc
							   */
	struct _s_proc * next_sending;/**
							   * next proc in the sending
							   * queue (q_sending)
							   */
    char p_name[16];
    TTY _tty;
    CONSOLE* _console;

};

typedef struct _s_proc PROCESS;

struct _s_tss{
    uint32_t backlink;
    uint32_t esp0;
    uint32_t ss0;   //low 16 bit available
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    /* segment registers low 16 bit available */
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;
    uint16_t trap;
    uint16_t iobase;
};

typedef struct _s_tss TSS;

#endif // MOONONS_PROC_H