
/*
 * Generic layer to access/use VGA text mode
 *
 * Compiled if "CFG_CONSOLE_VGA=1"
 */

#include <rks/kernel/init.h>
#include <rks/kernel/console/dispatcher.h>

#define VGA_BUFFER_ADDRESS 0xB8000
#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_BUFFER_SIZE (VGA_HEIGHT * VGA_WIDTH)

static void vga_flush_buffer (const struct screen_cell* buffer);

struct console_driver vga_drv = {
	.name = "vga",
	.flush = vga_flush_buffer,
};

/*
 * VGA context initializer
 *
 * -> void
 */
void
vga_init_context (void)
{
	/* Register this driver to start using it */
	console_register (&vga_drv);
}

/* Declare driver to be initialized */
DECLARE_DRIVER_INIT (vga_init_context);

/*
 * Framebuffer flusher helper
 * Use it when you want to "write" content on memory
 *
 * -> void
 */
void
vga_flush_buffer (const struct screen_cell* buffer)
{
	char* video = (char *) VGA_BUFFER_ADDRESS;

	for (__u16 i = 0; i < VGA_BUFFER_SIZE; i++)
	{
		video[i * 2] = buffer[i].character;
		video[i * 2 + 1] = buffer[i].color;
	}
}
