#ifndef _KERNEL_BOOT_FB_H
#define _KERNEL_BOOT_FB_H

#include <stddef.h>
#include <stdint.h>

/* Framebuffer information structure */
struct framebuffer_info
{

	/* Framebuffer virtual address pointer */
	uintptr_t* addr;

	/* Dimensions */
	uint32_t width;
	uint32_t height;

	/* Bytes per row */
	uint32_t pitch;

	/* Bits per pixel */
	uint8_t bpp;

	/* Formatter */
	struct
	{
		uint8_t red_shift;
		uint8_t green_shift;
		uint8_t blue_shift;
	} fmt;
};

#endif /* _KERNEL_BOOT_FB_H */
