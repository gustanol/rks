/*
 * Generic layer to access/use VGA text mode
 *
 * Compiled if "CFG_CONSOLE_VGA=1"
 */

#include <rks/drivers/console/vga.h>

#define VGA_BUFFER_ADDRESS 0xB8000

struct vga_ctx
{
	volatile __u16 *buffer;
	__u8            color;
};

static struct vga_ctx vga;
static char           vga_framebuffer[VGA_BUF_SIZE];

/*
 * VGA context initializer
 *
 * -> void
 */
void
vga_init_context (void)
{
	vga.buffer = (volatile __u16 *)vga_framebuffer;
	vga.color  = VGA_DEFAULT_COLOR;
}

/*
 * Put a character at the current position of framebuffer using the current color
 *
 * -> int: Return status
 * @void: Character to be written
 */
int
vga_put_char (char c, __u8 color, __u16 index)
{
	if (index >= VGA_BUF_SIZE)
		return 0;

	vga.buffer[index] = ((__u16)color << 8) | (__u8)(c);
	return 1;
}

void
vga_get_char (__u16 index, struct vga_buffer_char *bc)
{
	if (index >= VGA_BUF_SIZE)
		return;

	bc->character = (__u8)vga.buffer[index] >> 8;
	bc->color     = (__u8)vga.buffer[index] << 8;
}

/*
 * Color modifier helper
 * Notice there's no other way to modify the current color but this
 *
 * -> void
 * @color: Color to be set
 */
void
vga_set_color (__u8 color)
{
	vga.color = color;
}

/*
 * Replace all content of framebuffer for spaces
 *
 * -> void
 */
void
vga_clear_buffer (void)
{
	/* For now, clear buffer manually */
	for (__u16 i = 0; i < VGA_BUF_SIZE; i++)
		vga.buffer[i] = (__u16)0x20;
}

/*
 * Framebuffer flusher helper
 * Use it when you want to "write" content on memory
 *
 * -> void
 */
void
vga_flush_buffer (void)
{
	char       *video  = (char *)VGA_BUFFER_ADDRESS;
	const char *buffer = (const char *)vga.buffer;

	for (__u16 i = 0; i < VGA_BUF_SIZE; i++)
		video[i] = buffer[i];
}
