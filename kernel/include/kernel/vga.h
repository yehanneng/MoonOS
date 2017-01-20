#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

/*FOR vga text color
 * */
enum vga_color
{
    COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,

};

static inline unsigned char make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/* 8bit color | 8bit char */
static inline unsigned short make_vgaentry(char c,unsigned char color)
{
    unsigned short c16 = c;
    unsigned short color16 = color;
    return c16 | color16 << 8;
}

static const unsigned int VGA_WIDTH = 80;
static const unsigned int VGA_HEIGHT = 25;

static unsigned short* const VGA_MEMORY = (unsigned short*)0xB8000;

#endif // _KERNEL_VGA_H
