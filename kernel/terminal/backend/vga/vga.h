#ifndef _KERNEL_CONSOLE_VGA_BACKEND_H
#define _KERNEL_CONSOLE_VGA_BACKEND_H

#include <stddef.h>
#include <stdint.h>

static const uint32_t vga_height = 25;
static const uint32_t vga_width = 80;
static const uint32_t vga_cell_count = vga_height * vga_width;
static const uint32_t vga_default_color = 0x0F;
static const uint32_t vga_mem_addr = 0xB8000;

/* VGA screen cell */
struct vga_cell
{
	uint8_t character;
	uint8_t color;
} __attribute__ ( ( packed ) );

struct vga_ctx
{

	/* Buffer pointer */
	struct vga_cell* ptr;
};

#endif /* _KERNEL_CONSOLE_VGA_BACKEND_H */
