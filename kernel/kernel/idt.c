#include <kernel/idt.h>
#include <kernel/kernel.h>
#include <stdio.h>

void exception_handler(int vec_no, unsigned int err_code, int eip, int cs, int eflags)
{
    // int text_color = 0x74; /* 灰底红字 */
    char err_description[][64] = {	"#DE Divide Error",
        "#DB RESERVED",
        "—  NMI Interrupt",
        "#BP Breakpoint",
        "#OF Overflow",
        "#BR BOUND Range Exceeded",
        "#UD Invalid Opcode (Undefined Opcode)",
        "#NM Device Not Available (No Math Coprocessor)",
        "#DF Double Fault",
        "    Coprocessor Segment Overrun (reserved)",
        "#TS Invalid TSS",
        "#NP Segment Not Present",
        "#SS Stack-Segment Fault",
        "#GP General Protection",
        "#PF Page Fault",
        "—  (Intel reserved. Do not use.)",
        "#MF x87 FPU Floating-Point Error (Math Fault)",
        "#AC Alignment Check",
        "#MC Machine Check",
        "#XF SIMD Floating-Point Exception"
				};
    printf("Exception! --> %s\n",err_description[vec_no]);
    printf("EFLAGS = %x\n",eflags);
    printf("CS = %x \n",cs);
    printf("EIP = %x \n",eip);

    if (err_code != 0xffffffff) {
        printf("err code = %x\n",err_code);
    }
}

void init_idt_desc(GATE* p_gate, uint8_t desc_type, int_handler handler, unsigned char privilege)
{
    uint32_t base = (uint32_t)handler;
    p_gate->offset_low	= base & 0xFFFF;
    p_gate->selector	= SELECTOR_KERNEL_CS;
    p_gate->dcount		= 0;
    p_gate->attr		= desc_type | (privilege << 5);
    p_gate->offset_high	= (base >> 16) & 0xFFFF;
}

void init_8259A()
{
    out_byte(INT_M_CTL,	0x11);			/* Master 8259, ICW1. */
    out_byte(INT_S_CTL,	0x11);			/* Slave  8259, ICW1. */
    out_byte(INT_M_CTLMASK,	INT_VECTOR_IRQ0);	/* Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20. */
    out_byte(INT_S_CTLMASK,	INT_VECTOR_IRQ8);	/* Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28. */
    out_byte(INT_M_CTLMASK,	0x4);			/* Master 8259, ICW3. IR2 对应 '从8259'. */
    out_byte(INT_S_CTLMASK,	0x2);			/* Slave  8259, ICW3. 对应 '主8259' 的 IR2. */
    out_byte(INT_M_CTLMASK,	0x1);			/* Master 8259, ICW4. */
    out_byte(INT_S_CTLMASK,	0x1);			/* Slave  8259, ICW4. */

    out_byte(INT_M_CTLMASK,	0xF8);	/* Master 8259, OCW1. */
    out_byte(INT_S_CTLMASK,	0xBF);	/* Slave  8259, OCW1. */


    /* init 8253 */
    out_byte(TIMER_MODE, RATE_GENERATOR);
    out_byte(TIMER0, (uint8_t)TIMER_FREQ / HZ);
    out_byte(TIMER0, (uint8_t)((TIMER_FREQ / HZ) >> 8));
}
