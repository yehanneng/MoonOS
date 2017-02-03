#include <kernel/kernel.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/proc.h>
#include <stdio.h>
#include <kernel/interrupt/interrupt.h>

/* some global variable */
DESCRIPTOR* kgdt = 0;
GATE* kidt = 0;
TSS* tss = 0;
int k_reenter = 0;
PROCESS* p_proc_ready = 0;

PROCESS proc_table[1];



void kernel_init_gdt()
{
    kgdt = (DESCRIPTOR*) kmalloc(GDT_SIZE * sizeof(DESCRIPTOR));

    init_desc(&kgdt[0],0,0,0);
    init_desc(&kgdt[1], 0, 0xfffff, DA_CR |DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[2], 0, 0xfffff, DA_DRW|DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[3], 0xB8000, 0xffff, DA_DRW | DA_DPL3);
    tss = (TSS*)kmalloc(sizeof(TSS));
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

	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 8], DA_386IGate,
	// 	hwint08, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 9], DA_386IGate,
	// 	hwint09, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 10], DA_386IGate,
	// 	hwint10, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 11], DA_386IGate,
	// 	hwint11, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 12], DA_386IGate,
	// 	hwint12, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 13], DA_386IGate,
	// 	hwint13, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 14], DA_386IGate,
	// 	hwint14, PRIVILEGE_KRNL);
    //
	// init_idt_desc(&kidt[INT_VECTOR_IRQ0 + 15], DA_386IGate,
	// 	hwint15, PRIVILEGE_KRNL);

    // init_idt_desc(&kidt[INT_VECTOR_SYS_CALL], DA_386IGate, sys_call, PRIVILEGE_USER);

    init_irq_table();

    char kidtr[6];

    uint16_t* p_idt_limit = (uint16_t*)(&kidtr[0]);
    uint32_t* p_idt_base  = (uint32_t*)(&kidtr[2]);

    *p_idt_limit = sizeof(GATE) * IDT_SIZE - 1;
    *p_idt_base = (uint32_t)kidt;

    load_idt(kidtr);
}
