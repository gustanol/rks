#ifndef KERNEL_BOOT_FB_H
#define KERNEL_BOOT_FB_H

#include <rks/kernel/types.h>

/* Framebuffer information structure */
struct framebuffer_info
{
	void* addr;
	u32 width;
	u32 height;

	/* Bytes per row */
	u32 pitch;

	/* Bits per pixel */
	ubyte_t bpp;

	struct
	{
		ubyte_t red_shift;
		ubyte_t green_shift;
		ubyte_t blue_shift;
	} fmt;
};

#endif /* KERNEL_BOOT_FB_H */
