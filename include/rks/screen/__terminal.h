#ifndef SCREEN_H
#define SCREEN_H

#include <rks/types.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

struct terminal_t
{
	volatile __u16 *buffer; /* Framebuffer */
	__u32           x, y;
	__u32           color;
};

void __terminal_init(void);
void __terminal_putchar(char c);
void __terminal_write(const char *c);
void __terminal_clear(void);

#endif /* SCREEN_H */
