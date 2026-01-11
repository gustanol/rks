#ifndef SCREEN_H
#define SCREEN_H

#include <rks/types.h>

#define VGA_WIDTH    80
#define VGA_HEIGHT   25
#define VGA_ADD      0xB8000
#define VGA_TEXT_BOX (VGA_HEIGHT * VGA_WIDTH * 2)

struct terminal_t
{
	volatile __u16 *buffer; /* Framebuffer */
	__u32           x, y;
	__u8            color;
};

void _terminal_init(void);
void _terminal_putchar(char c);
void _terminal_write(const char *c);
void __terminal_flush(void);

#endif /* SCREEN_H */
