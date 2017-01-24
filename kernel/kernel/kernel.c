#include <kernel/tty.h>
#include <stdlib.h>

void kernel_early(void)
{
    terminal_initialize();
}

void kernel_main(void)
{
    printf("Hello Kernel world!!\n");
}
