#ifndef VGA_H
#define VGA_H

#include <rks/types.h>

#define VGA_HEIGHT        25
#define VGA_WIDTH         80
#define VGA_BUF_SIZE      (2 * VGA_WIDTH * VGA_HEIGHT)
#define VGA_DEFAULT_COLOR 0x0F

struct vga_buffer_char
{
	__u8 character;
	__u8 color;
};

void vga_init_context (void);
void vga_set_color (__u8 color);
int  vga_put_char (char c, __u8 color, __u16 index);
void vga_get_char (__u16 index, struct vga_buffer_char *bc);
void vga_clear_buffer (void);
void vga_flush_buffer (void);

#endif /* VGA_H */
