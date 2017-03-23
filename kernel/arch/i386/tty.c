#include <kernel/tty.h>
#include <kernel/vga.h>

unsigned int terminal_row;
unsigned int terminal_column;
unsigned char terminal_color;
unsigned short* terminal_buffer;

#ifdef __cplusplus
extern "C" {
#endif

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;

    for (unsigned int y = 0; y < VGA_HEIGHT; y++) {
        for (unsigned int x = 0; x < VGA_WIDTH; x++) {
            unsigned int index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(unsigned char color) {
    terminal_color = color;
}

void terminal_putentryat(char c, unsigned char color, unsigned int x, unsigned int y) {
    const unsigned int index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\r') {
        /* code */
        terminal_column = -1;
    } else if (c == '\n') {
        /* code enter */
        terminal_row++;
        terminal_column = -1;
    } else if (c == '\b') {
        terminal_column--;
        if (terminal_column <= 0) {
            terminal_column = VGA_WIDTH - 1;
            terminal_row--;
        }
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        // move_cursor(terminal_row, terminal_column);
        return;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void terminal_write(const char *data, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}


#ifdef __cplusplus
}
#endif