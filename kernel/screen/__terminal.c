#include <rks/screen/__terminal.h>
#include <rks/types.h>

struct terminal_t *terminal_p;

/*
 * Low-level function to Initialize the virtual terminal
 *
 * -> `void`
 */
void __terminal_init(void)
{
	/* Zero-initialized screen buffer */
	static char screen_buffer[VGA_WIDTH * VGA_HEIGHT * 2];

	/* Populate the global-state terminal pointer */
	terminal_p->buffer = (__u16 *)screen_buffer;
	terminal_p->x = terminal_p->y = 0;
	terminal_p->color             = 0;
}

/*
 * Low-level function to put a character at the current terminal coordinate
 *
 * -> `void`
 * @c: The character
 */
void __terminal_putchar(char c)
{
	/* Write the character at the current position within framebuffer */
	terminal_p->buffer[terminal_p->y * VGA_HEIGHT + terminal_p->x] = c;

	if (terminal_p->x == VGA_WIDTH)
	{
		terminal_p->y++;
		terminal_p->x = 0;
	}

	else
		terminal_p->x++;
}

/*
 * Low-level function to write a string into the framebuffer
 */
void __terminal_write(const char *c)
{
	for (; *c != '\n'; c++)
		__terminal_putchar(*c);
}
