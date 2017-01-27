#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <grub/multiboot.h>
#include <kernel/memory.h>
#include <stdlib.h>
#include <stdint.h>

/** do all the init process **/
void kernel_early(multiboot_info_t* bootInfo)
{
    terminal_initialize();
    // load memory info
    parse_memory_info(bootInfo);
    // setup system gdt
    kernel_init_gdt();
}

void kernel_main(void)
{
    printf("Hello Kernel world!!\n");
}
