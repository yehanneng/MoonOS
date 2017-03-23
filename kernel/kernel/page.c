#include <kernel/page.h>
#include <stdint.h>
#include <stdio.h>
#include <kernel/kernel.h>

// pate table addr
uint32_t* _page_derectory = (uint32_t*)PDE_BASE;
uint32_t* _page_table = (uint32_t*) PTE_BASE;;

void kernel_setup_page(unsigned int totalMemory)
{
    uint32_t total_memory = totalMemory;
    uint32_t i,j;
    for(i = 0;i < ENTRY_SIZE;i++){
        _page_derectory[i] = 0x00000002;
    }

    uint32_t* p_ptb = _page_table;
    for(i = 0;i < ENTRY_SIZE && total_memory > 0;i++){
        for(j = 0; j < ENTRY_SIZE && total_memory > 0;j++){
            p_ptb[j] = (i << 22) + (j << 12) + 3;
            total_memory -= 0x1000;
        }
        _page_derectory[i] = (uint32_t)p_ptb | 3;
        p_ptb += 1024;
    }
    set_cr3((uint32_t)_page_derectory);
}
