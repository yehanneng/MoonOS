#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#ifdef __cplusplus
extern "C" {
#endif

void terminal_initialize(void);

void terminal_putchar(char c);

void terminal_write(const char *data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif // _KERNEL_TTY_H
