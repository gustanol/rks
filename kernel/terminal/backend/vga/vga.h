#ifndef _KERNEL_CONSOLE_VGA_BACKEND_H
#define _KERNEL_CONSOLE_VGA_BACKEND_H

#include <stddef.h>
#include <stdint.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_DEFAULT_COLOR 0x0F
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
		struct vga_cell* ptr;

		struct
		{

			/* Dimensions */
			size_t width;
			size_t height;

			/* Amount of cells */
			size_t cell_count;

			/* Current color byte */
			uint8_t current_color;
		} attr;

		struct
		{
			uint32_t x;
			uint32_t y;
		} coordinates;

	} screen;

	/* Actual memory address to flush */
	uintptr_t maddr;

	/* Is it ready? */
	bool ready;
};

#endif /* _KERNEL_CONSOLE_VGA_BACKEND_H */
