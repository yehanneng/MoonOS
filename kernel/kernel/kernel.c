#include <kernel/kernel.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/proc.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/interrupt/interrupt.h>
#include <kernel/memory.h>
#include <liballoc.h>
#include <string.h>
#include <assert.h>

#include "tasks/InputTask.h"
#include "tasks/ticktask.h"
#include "tasks/harddisktask.h"
#include "tasks/filesystemtask.h"

/* some global variable */
DESCRIPTOR* kgdt = 0;
GATE* kidt = 0;
TSS* tss = 0;
int k_reenter = -1;
PROCESS* p_proc_ready = 0;
PROCESS* focus_proc = NULL;

PROCESS proc_table[NR_TASKS];

/* private function */
static int  msg_send(PROCESS* current, int dest, MESSAGE* m);
static int  msg_receive(PROCESS* current, int src, MESSAGE* m);
static int  deadlock(int src, int dest);
static void unblock(PROCESS* p);
static void block(PROCESS* p);

// for debug
void TestA();
void TestB();

TASK_T task_table[NR_TASKS] = {
        {input_task_main, TASK_STACK_SIZE,"InputTask",10},
        {hd_task_main, TASK_STACK_SIZE, "HDDriver",20},
        {tick_task_main, TASK_STACK_SIZE, "TICK", 20},
        {file_task_main, TASK_STACK_SIZE, "FileTask", 20},
        {TestB, TASK_STACK_SIZE, "TestB", 20}
};


void kernel_init_gdt()
{
    kgdt = (DESCRIPTOR*) kmalloc(GDT_SIZE * sizeof(DESCRIPTOR));

    init_desc(&kgdt[0],0,0,0);
    init_desc(&kgdt[1], 0, 0xfffff, DA_CR |DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[2], 0, 0xfffff, DA_DRW|DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[3], 0xB8000, 0xffff, DA_DRW | DA_DPL3);
    tss = (TSS*)kmalloc(sizeof(TSS));
    memset(tss,0, sizeof(TSS));
    tss->ss0 = SELECTOR_KERNEL_DS;
    tss->iobase = sizeof(TSS);

    init_desc(&kgdt[INDEX_TSS], (uint32_t)tss, sizeof(TSS) - 1, DA_386TSS);

    char gdt_ptr[6];
    uint16_t* p_gdt_limit = (uint16_t*)(&gdt_ptr[0]);
    uint32_t* p_gdt_base  = (uint32_t*)(&gdt_ptr[2]);

    *p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
    *p_gdt_base = (uint32_t)kgdt;

    set_gdt(gdt_ptr);

    load_tss(SELECTOR_TSS);

}

void kernel_init_idt()
{
    init_8259A();

    kidt = (GATE*) kmalloc(IDT_SIZE * sizeof(GATE));

    init_idt_desc(&kidt[INT_VECTOR_DIVIDE],	DA_386IGate,
                  divide_error,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_DEBUG],		DA_386IGate,
                  single_step_exception,	PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_NMI],		DA_386IGate,
                  nmi,			PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_BREAKPOINT],	DA_386IGate,
                  breakpoint_exception,	PRIVILEGE_USER);

    init_idt_desc(&kidt[INT_VECTOR_OVERFLOW],	DA_386IGate,
                  overflow,			PRIVILEGE_USER);

    init_idt_desc(&kidt[INT_VECTOR_BOUNDS],	DA_386IGate,
                  bounds_check,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_INVAL_OP],	DA_386IGate,
                  inval_opcode,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_COPROC_NOT],	DA_386IGate,
                  copr_not_available,	PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_DOUBLE_FAULT],	DA_386IGate,
                  double_fault,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_COPROC_SEG],	DA_386IGate,
                  copr_seg_overrun,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_INVAL_TSS],	DA_386IGate,
                  inval_tss,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_SEG_NOT],	DA_386IGate,
                  segment_not_present,	PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_STACK_FAULT],	DA_386IGate,
                  stack_exception,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_PROTECTION],	DA_386IGate,
                  general_protection,	PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_PAGE_FAULT],	DA_386IGate,
                  page_fault,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_COPROC_ERR],	DA_386IGate,
                  copr_error,		PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 0],      DA_386IGate,
                  hwint00,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 1],      DA_386IGate,
                  hwint01,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 2],      DA_386IGate,
                  hwint02,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 3],      DA_386IGate,
                  hwint03,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 4],      DA_386IGate,
                  hwint04,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 5],      DA_386IGate,
                  hwint05,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 6],      DA_386IGate,
                  hwint06,                  PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 7],      DA_386IGate,
                  hwint07,                  PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 8], DA_386IGate,
	 	hwint08, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 9], DA_386IGate,
	 	hwint09, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 10], DA_386IGate,
	 	hwint10, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 11], DA_386IGate,
	 	hwint11, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 12], DA_386IGate,
	 	hwint12, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 13], DA_386IGate,
	 	hwint13, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 14], DA_386IGate,
	 	hwint14, PRIVILEGE_KRNL);

	 init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 15], DA_386IGate,
	 	hwint15, PRIVILEGE_KRNL);

    init_idt_desc(&kidt[INT_VECTOR_SYS_CALL], DA_386IGate, sys_call, PRIVILEGE_USER);

    init_irq_table();

    char kidtr[6];

    uint16_t* p_idt_limit = (uint16_t*)(&kidtr[0]);
    uint32_t* p_idt_base  = (uint32_t*)(&kidtr[2]);

    *p_idt_limit = sizeof(GATE) * IDT_SIZE - 1;
    *p_idt_base = (uint32_t)kidt;

    load_idt(kidtr);
}

void kernel_schedule_process()
{
    p_proc_ready->ticks--;
    if(k_reenter != 0) { // interrupt reenter
        return;
    }
    PROCESS* p;
    int greates_ticks = 0;

    while (!greates_ticks) {
        for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
            if (p->p_flags == 0) {
                if (p->ticks > greates_ticks) {
                    greates_ticks = p->ticks;
                    p_proc_ready = p;
                }
            }
        }

        if (!greates_ticks) {
            for (p = proc_table; p < proc_table + NR_TASKS + NR_PROCS; p++) {
                if (p->p_flags == 0)
                    p->ticks = p->priority;
            }
        }
    }
}

int kernel_proc2pid(PROCESS* proc)
{
    return proc - proc_table;
}

int kernel_ldt_seg_linear(PROCESS* p, int idx)
{
    assert(idx < LDT_SIZE);
    DESCRIPTOR* desc = &p->ldts[idx];

    return desc->base_high << 24 | desc->base_mid << 16 | desc->base_low;
}

PROCESS* kernel_pid2proc(int pid)
{
    assert(pid < NR_TASKS + NR_PROCS);
    return proc_table + pid;
}

void* kernel_va2la(int pid, void* va)
{
    PROCESS* proc = kernel_pid2proc(pid);
    uint32_t seg_base = kernel_ldt_seg_linear(proc, INDEX_LDT_RW);
    uint32_t linear_addr = seg_base + (uint32_t)va;

    if (pid < NR_TASKS) {
        assert(linear_addr == (uint32_t) va);
    }
    return (void*) linear_addr;
}

int kernel_sendrec(int function, int src_dest, MESSAGE* msg, PROCESS* proc)
{
    assert(k_reenter == 0);	/* make sure we are not in ring0 */
    assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
           src_dest == ANY ||
           src_dest == INTERRUPT);
    int ret = 0;
    int caller = kernel_proc2pid(proc);
    MESSAGE* mla = (MESSAGE*) kernel_va2la(caller, msg);
    mla->source = caller;

    assert(mla->source != src_dest);

    /**
	 * Actually we have the third message type: BOTH. However, it is not
	 * allowed to be passed to the kernel directly. Kernel doesn't know
	 * it at all. It is transformed into a SEND followed by a RECEIVE
	 * by `send_recv()'.
	 */
    if (function == SEND) {
        ret = msg_send(proc, src_dest, msg);
        if (ret != 0)
            return ret;
    }
    else if (function == RECEIVE) {
        ret = msg_receive(proc, src_dest, msg);
        if (ret != 0)
            return ret;
    }
    else {
        assert(function == BOTH);
    }

    return 0;
}

static int msg_send(PROCESS* current, int dest, MESSAGE* m)
{
    PROCESS* sender = current;
    PROCESS* p_dest = proc_table + dest;
    assert(kernel_proc2pid(sender) != dest);
    if (deadlock(kernel_proc2pid(sender), dest)) { //dead block
        printf("dead block\n");
        abort();
    }

    if((p_dest->p_flags & RECEIVING) && /* dest is waiting for the msg */
       (p_dest->p_recvfrom == kernel_proc2pid(sender) ||
        p_dest->p_recvfrom == ANY)) {
        assert(p_dest->p_msg);
        assert(m);
        memcpy(kernel_va2la(dest, p_dest->p_msg),
               kernel_va2la(kernel_proc2pid(sender), m),
               sizeof(MESSAGE));
        p_dest->p_msg = 0;
        p_dest->p_flags &= ~RECEIVING; /* dest has received the msg */
        p_dest->p_recvfrom = NO_TASK;
        unblock(p_dest);

        assert(p_dest->p_flags == 0);
        assert(p_dest->p_msg == 0);
        assert(p_dest->p_recvfrom == NO_TASK);
        assert(p_dest->p_sendto == NO_TASK);
        assert(sender->p_flags == 0);
        assert(sender->p_msg == 0);
        assert(sender->p_recvfrom == NO_TASK);
        assert(sender->p_sendto == NO_TASK);
    }
    else { /* dest is not waiting for the msg */
        sender->p_flags |= SENDING;
        assert(sender->p_flags == SENDING);
        sender->p_sendto = dest;
        sender->p_msg = m;

        /* append to the sending queue */
        PROCESS * p;
        if (p_dest->q_sending) {
            p = p_dest->q_sending;
            while (p->next_sending)
                p = p->next_sending;
            p->next_sending = sender;
        }
        else {
            p_dest->q_sending = sender;
        }
        sender->next_sending = 0;

        block(sender);

        assert(sender->p_flags == SENDING);
        assert(sender->p_msg != 0);
        assert(sender->p_recvfrom == NO_TASK);
        assert(sender->p_sendto == dest);
    }

    return 0;
}

static int msg_receive(PROCESS* current, int src, MESSAGE* m)
{
    PROCESS* p_who_wanna_recv = current; /**
											 * This name is a little bit
											 * wierd, but it makes me
											 * think clearly, so I keep
											 * it.
											 */
    PROCESS* p_from = 0; /* from which the message will be fetched */
    PROCESS* prev = 0;
    int copyok = 0;

    assert(kernel_proc2pid(p_who_wanna_recv) != src);

    if ((p_who_wanna_recv->has_int_msg) &&
        ((src == ANY) || (src == INTERRUPT))) {
        /* There is an interrupt needs p_who_wanna_recv's handling and
        * p_who_wanna_recv is ready to handle it.
        */
        MESSAGE msg;
        memset(&msg, 0 , sizeof(MESSAGE));
        msg.source = INTERRUPT;
        msg.type = HARD_INT;

        assert(m);

        memcpy(kernel_va2la(kernel_proc2pid(p_who_wanna_recv), m), &msg,
               sizeof(MESSAGE));

        p_who_wanna_recv->has_int_msg = 0;

        assert(p_who_wanna_recv->p_flags == 0);
        assert(p_who_wanna_recv->p_msg == 0);
        assert(p_who_wanna_recv->p_sendto == NO_TASK);
        assert(p_who_wanna_recv->has_int_msg == 0);

        return 0;
    }

    /* Arrives here if no interrupt for p_who_wanna_recv. */
    if (src == ANY) {
        /* p_who_wanna_recv is ready to receive messages from
        * ANY proc, we'll check the sending queue and pick the
        * first proc in it.
        */
        if (p_who_wanna_recv->q_sending) {
            p_from = p_who_wanna_recv->q_sending;
            copyok = 1;

            assert(p_who_wanna_recv->p_flags == 0);
            assert(p_who_wanna_recv->p_msg == 0);
            assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
            assert(p_who_wanna_recv->p_sendto == NO_TASK);
            assert(p_who_wanna_recv->q_sending != 0);
            assert(p_from->p_flags == SENDING);
            assert(p_from->p_msg != 0);
            assert(p_from->p_recvfrom == NO_TASK);
            assert(p_from->p_sendto == kernel_proc2pid(p_who_wanna_recv));
        }
    }
    else if (src >= 0 && src < NR_TASKS + NR_PROCS) {
        /* p_who_wanna_recv wants to receive a message from
        * a certain proc: src.
        */
        p_from = &proc_table[src];

        if ((p_from->p_flags & SENDING) &&
            (p_from->p_sendto == kernel_proc2pid(p_who_wanna_recv))) {
            /* Perfect, src is sending a message to
            * p_who_wanna_recv.
            */
            copyok = 1;

            PROCESS* p = p_who_wanna_recv->q_sending;

            assert(p); /* p_from must have been appended to the
					   * queue, so the queue must not be NULL
					   */

            while (p) {
                assert(p_from->p_flags & SENDING);

                if (kernel_proc2pid(p) == src) /* if p is the one */
                    break;

                prev = p;
                p = p->next_sending;
            }

            assert(p_who_wanna_recv->p_flags == 0);
            assert(p_who_wanna_recv->p_msg == 0);
            assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
            assert(p_who_wanna_recv->p_sendto == NO_TASK);
            assert(p_who_wanna_recv->q_sending != 0);
            assert(p_from->p_flags == SENDING);
            assert(p_from->p_msg != 0);
            assert(p_from->p_recvfrom == NO_TASK);
            assert(p_from->p_sendto == kernel_proc2pid(p_who_wanna_recv));
        }
    }

    if (copyok) {
        /* It's determined from which proc the message will
        * be copied. Note that this proc must have been
        * waiting for this moment in the queue, so we should
        * remove it from the queue.
        */
        if (p_from == p_who_wanna_recv->q_sending) { /* the 1st one */
            assert(prev == 0);
            p_who_wanna_recv->q_sending = p_from->next_sending;
            p_from->next_sending = 0;
        }
        else {
            assert(prev);
            prev->next_sending = p_from->next_sending;
            p_from->next_sending = 0;
        }

        assert(m);
        assert(p_from->p_msg);

        /* copy the message */
        memcpy(kernel_va2la(kernel_proc2pid(p_who_wanna_recv), m),
               kernel_va2la(kernel_proc2pid(p_from), p_from->p_msg),
               sizeof(MESSAGE));

        p_from->p_msg = 0;
        p_from->p_sendto = NO_TASK;
        p_from->p_flags &= ~SENDING;
        unblock(p_from);
    }
    else {  /* nobody's sending any msg */
        /* Set p_flags so that p_who_wanna_recv will not
        * be scheduled until it is unblocked.
        */
        p_who_wanna_recv->p_flags |= RECEIVING;

        p_who_wanna_recv->p_msg = m;
        p_who_wanna_recv->p_recvfrom = src;
        block(p_who_wanna_recv);

        assert(p_who_wanna_recv->p_flags == RECEIVING);
        assert(p_who_wanna_recv->p_msg != 0);
        assert(p_who_wanna_recv->p_recvfrom != NO_TASK);
        assert(p_who_wanna_recv->p_sendto == NO_TASK);
        assert(p_who_wanna_recv->has_int_msg == 0);
    }

    return 0;
}

static int deadlock(int src, int dest)
{
    PROCESS* p = proc_table + dest;
    while (1) {
        if (p->p_flags & SENDING) {
            if (p->p_sendto == src) {
                /* dead block */
                assert(p->p_sendto == src);
                return 1;
            }
            p = proc_table + p->p_sendto;
        } else {
            break;
        }
    }
    return 0;
}

static void unblock(PROCESS* p)
{
    assert(p->p_flags == 0);
}

static void block(PROCESS* p)
{
    assert(p->p_flags);
    kernel_schedule_process();
}

/**
 * there is only sys call like Minix
 * and this is send and receive message
 * @param sys_call_vector keep this ,maybe future this will be used
 * @param arg1
 * @param arg2
 * @param arg3
 * @param caller
 * @return
 */
void* kernel_sys_call(int sys_call_vector, void* arg1, void* arg2, void* arg3, PROCESS* caller)
{
    return (void*) kernel_sendrec((int) arg1, (int) arg2, (MESSAGE*) arg3, caller);
}

/****** only for debug ******/
void TestA()
{
    int times = 0;
    MESSAGE _msg;
    while(1){
        times++;
        int ret = send_recv(RECEIVE, ANY, &_msg);
        if (ret == 0) {
            _msg.RETVAL = 2460;
            send_recv(SEND, _msg.source, &_msg);
        }
    }
}

void TestB()
{
    int times = 0;
    MESSAGE _msg;
    int tick = get_ticket();
//    printf("get ticks = %d\n", tick);

    while(1){
        if(times < 100){
//            printf("a");
            times++;
        }
    }
}

void kernel_init_internal_process()
{
    uint16_t ldt_selector = SELECTOR_LDT_FIRST;
    PROCESS* p;

    for (int i = 0; i < NR_TASKS; ++i) {
        p = proc_table + i;
        memset(p, 0, sizeof(PROCESS));
        p->ldt_sel = ldt_selector;
        // code segment
        memcpy(&p->ldts[0], &kgdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
        p->ldts[0].attr1 = DA_C | (PRIVILEGE_TASK << 5);

        memcpy(&p->ldts[1], &kgdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
        p->ldts[1].attr1 = DA_DRW | (PRIVILEGE_TASK << 5);

        p->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        p->regs.gs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;

        p->regs.eip = (uint32_t)task_table[i].entry_point;
        p->regs.esp = memory_alloc_4k(TASK_STACK_SIZE) + TASK_STACK_SIZE;
        p->regs.eflags = 0x1202;
        p->pid = i;

        p->_input_buffer.count = 0;
        p->_input_buffer._p_head = p->_input_buffer._p_tail = p->_input_buffer._buff;

        p->ticks = p->priority = task_table[i].privilege;

        p->p_flags = 0;
        p->p_msg = 0;
        p->p_recvfrom = p->p_sendto = NO_TASK;
        p->q_sending = p->next_sending = 0;
        p->has_int_msg = 0;

        init_desc(&kgdt[ldt_selector >> 3],(uint32_t)p->ldts, LDT_SIZE * sizeof(DESCRIPTOR) - 1,DA_LDT);
        ldt_selector += 8;
    }
    focus_proc = proc_table + 1;
    p_proc_ready = proc_table;
}


PROCESS* kernel_getFocusProcess()
{
    return focus_proc;
}
void kernel_info_task_from_interrupt(int pid)
{
    PROCESS* p_proc = kernel_pid2proc(pid);

    if ((p_proc->p_flags & RECEIVING) && /* dest is waiting for the msg */
        ((p_proc->p_recvfrom == INTERRUPT) || (p_proc->p_recvfrom == ANY))) {
        p_proc->p_msg->source = INTERRUPT;
        p_proc->p_msg->type = HARD_INT;
        p_proc->p_msg = 0;
        p_proc->has_int_msg = 0;
        p_proc->p_flags &= ~RECEIVING; /* dest has received the msg */
        p_proc->p_recvfrom = NO_TASK;
        assert(p_proc->p_flags == 0);
        unblock(p_proc);

        assert(p_proc->p_flags == 0);
        assert(p_proc->p_msg == 0);
        assert(p_proc->p_recvfrom == NO_TASK);
        assert(p_proc->p_sendto == NO_TASK);
    }
    else {
        p_proc->has_int_msg = 1;
    }
}