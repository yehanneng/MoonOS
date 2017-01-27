#include <kernel/kernel.h>
#include <kernel/gdt.h>
#include <stdio.h>


DESCRIPTOR* kgdt = 0;

void kernel_init_gdt()
{
    kgdt = (DESCRIPTOR*) kmalloc(GDT_SIZE * sizeof(DESCRIPTOR));

    init_desc(&kgdt[0],0,0,0);
    init_desc(&kgdt[1], 0, 0xfffff, DA_CR |DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[2], 0, 0xfffff, DA_DRW|DA_32 | DA_LIMIT_4K);
    init_desc(&kgdt[3], 0xB8000, 0xffff, DA_DRW | DA_DPL3);

    char gdt_ptr[6];
    uint16_t* p_gdt_limit = (uint16_t*)(&gdt_ptr[0]);
    uint32_t* p_gdt_base  = (uint32_t*)(&gdt_ptr[2]);

    *p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
    *p_gdt_base = (uint32_t)kgdt;

    set_gdt(gdt_ptr);

}
