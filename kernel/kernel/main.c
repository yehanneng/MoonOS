
#include <kernel/tty.h>
#include <grub/multiboot.h>
#include <kernel/memory.h>
#include <stdlib.h>
#include <stdint.h>

/** do all the init process **/
void kernel_early(multiboot_info_t* bootInfo)
{
    terminal_initialize();

    parse_memory_info(bootInfo);
}

void kernel_main(void)
{
    printf("Hello Kernel world!!\n");
    void* addr = kmalloc(10);
    void* addr2 = kmalloc(20);
    printf("addr of malloc = %x\n | addr2 = %x", addr, addr2);
}
