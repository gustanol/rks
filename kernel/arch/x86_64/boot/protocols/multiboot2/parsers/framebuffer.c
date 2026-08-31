#include <kernel/boot/fb.h>

// #include <rks/lib/boot/multiboot2.h>

/**
 * Parses the given framebuffer to the global state boot info
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct multiboot2_tag_framebuffer* tag`: Multiboot2 framebuffer tag from MBI2
 *   2. `struct framebuffer_info* framebuffer`: Frambuffer kernel global state
 * Callers:
 *   - `_parse_mbi2`
 **/
void
__parse_framebuffer ( struct multiboot2_tag_framebuffer* tag, struct framebuffer_info* framebuffer )
{

	/* Framebuffer base physical address */
	framebuffer->addr = tag->framebuffer_addr;

	/* Screen width */
	framebuffer->width = tag->framebuffer_width;

	/* Screen height */
	framebuffer->height = tag->framebuffer_height;

	/* Screen pitch (bytes per row) */
	framebuffer->pitch = tag->framebuffer_pitch;

	/* Screen BPP (bits per pixel) */
	framebuffer->bpp = tag->framebuffer_bpp;

	/* Format description */
	framebuffer->fmt.red_shift = tag->framebuffer_red_field_position;
	framebuffer->fmt.green_shift = tag->framebuffer_green_field_position;
	framebuffer->fmt.blue_shift = tag->framebuffer_blue_field_position;
}
