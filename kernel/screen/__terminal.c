#include <rks/screen/__terminal.h>
#include <rks/types.h>

struct terminal_t *terminal_p;

/*
 * Low-level function to Initialize the virtual terminal
 *
 * -> `void`
 */
void _terminal_init(void)
{
	/* Zero-initialized screen buffer */
	static char screen_buffer[VGA_TEXT_BOX];

	/* Populate the global-state terminal pointer */
	terminal_p->buffer = (__u16 *)screen_buffer;
	terminal_p->x = terminal_p->y = 0;
	terminal_p->color             = 0x0F;
}

/*
 * Low-level function to put a character at the current terminal coordinate
 *
 * -> `void`
 * @c: The character
 */
void _terminal_putchar(char c)
{
	/* Write the character at the current position within framebuffer */
	terminal_p->buffer[terminal_p->y * VGA_WIDTH + terminal_p->x] =
	    ((__u16)terminal_p->color << 8) | (__u8)c;

	if (terminal_p->x == VGA_WIDTH - 1)
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
void _terminal_write(const char *c)
{
	for (; *c != '\0'; c++)
		_terminal_putchar(*c);
}

void __terminal_flush(void)
{
	char       *video = (char *)VGA_ADD;
	const char *msg   = (const char *)terminal_p->buffer;

	for (int i = 0; msg[i] != '\0'; i++)
		video[i] = msg[i];
}
