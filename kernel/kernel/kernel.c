#include <kernel/tty.h>
#include <stdlib.h>

void kernel_early(void)
{
    terminal_initialize();
}

void kernel_main(void)
{
    terminal_write("Hello World",11);
    printf("Hellp\n");
}
