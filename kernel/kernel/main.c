#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <grub/multiboot.h>
#include <kernel/memory.h>
#include <kernel/page.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t totalMemory = 0;

/** do all the init process **/
void kernel_early(multiboot_info_t* bootInfo)
{
    terminal_initialize();
    // load memory info
    totalMemory = parse_memory_info(bootInfo);
    // setup system gdt
    kernel_init_gdt();
    // setup page table
    kernel_setup_page(totalMemory);
}

void kernel_main(void)
{
    printf("Hello Kernel world!!\n");
}
