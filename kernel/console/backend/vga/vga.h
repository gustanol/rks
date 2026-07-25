#ifndef _KERNEL_CONSOLE_VGA_BACKEND_H
#define _KERNEL_CONSOLE_VGA_BACKEND_H

#include <stddef.h>
#include <stdint.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_MADDR 0xB8000

/* VGA screen cell */
struct vga_cell
{
	uint8_t character;
	uint8_t color;
};

/* VGA global context */
struct vga_ctx
{
	struct
	{

		/* Array of cells */
		struct vga_cell** ptr;

		/* Amount of cells */
		size_t cell_count;
	} screen;

	/* Actual memory address to flush */
	uintptr_t maddr;

	/* Is it ready? */
	bool ready;
};

#endif /* _KERNEL_CONSOLE_VGA_BACKEND_H */
