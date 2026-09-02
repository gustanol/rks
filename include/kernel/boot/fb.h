#ifndef _KERNEL_BOOT_FB_H
#define _KERNEL_BOOT_FB_H

#include <stddef.h>
#include <stdint.h>

/* Framebuffer information structure */
struct framebuffer_info
{

	/* Framebuffer virtual address pointer */
	uint64_t addr;

	/* Dimensions */
	uint32_t width;
	uint32_t height;

	/* Bytes per row */
	uint32_t pitch;

	/* Bits per pixel */
	uint8_t bpp;

	struct
	{
		struct
		{
			uint8_t pos;
			uint8_t mask;
		} red;

		struct
		{
			uint8_t pos;
			uint8_t mask;
		} green;

		struct
		{
			uint8_t pos;
			uint8_t mask;
		} blue;
	} rgb;
};

#endif /* _KERNEL_BOOT_FB_H */
