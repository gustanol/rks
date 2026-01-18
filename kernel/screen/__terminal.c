#include <rks/math.h>
#include <rks/screen/__terminal.h>
#include <rks/string.h>
#include <rks/types.h>

/* Zero-initialized screen buffer */
static char              screen_buffer[VGA_BUF_SIZE];
static struct terminal_t terminal_p;
static inline bool       _terminal_symbol(char c);

/*
 * Initialize the virtual terminal
 *
 * -> `void`
 */
void _terminal_init(void)
{
	/* Populate the global-state terminal pointer */
	terminal_p.buffer = (__u16 *)screen_buffer;
	terminal_p.x = terminal_p.y = 0;
	terminal_p.color            = DEFAULT_VGA_COLOR;
}

/*
 * Write a character at the current terminal coordinate
 *
 * -> `void`
 * @c: The character
 */
void _terminal_putchar(char **c)
{
	if (_terminal_symbol(**c))
		return;

	switch (**c)
	{
		case '\n':
		{
			terminal_p.x = 0;
			terminal_p.y++;

			if (terminal_p.y >= VGA_HEIGHT)
				_terminal_scroll();
			break;
		}
	}

	/* Write the character at the current position within framebuffer */
	terminal_p.buffer[terminal_p.y * VGA_WIDTH + terminal_p.x] =
	    ((__u16)terminal_p.color << 8) | (__u8) * *c;

	terminal_p.x++;
	if (terminal_p.x >= VGA_WIDTH)
	{
		terminal_p.x = 0;
		terminal_p.y++;
	}

	if (terminal_p.y >= VGA_HEIGHT)
		_terminal_scroll();
}

/*
 * Write a string into the framebuffer
 *
 * -> `void`
 * @c: Pointer to the string
 */
void _terminal_write(char *c)
{
	for (; *c != '\0'; c++)
		_terminal_putchar(&c);
}

/*
 * Scroll terminal to the next line
 *
 * -> `void`
 */
void _terminal_scroll(void)
{
	/* Copy each row to its previous one */
	for (__u16 y = 1; y < VGA_HEIGHT; y++)
		for (__u16 x = 0; x < VGA_WIDTH; x++)
			terminal_p.buffer[(y - 1) * VGA_WIDTH + x] =
			    terminal_p.buffer[y * VGA_WIDTH + x];

	/* Clear the last row */
	for (__u16 x = 0; x < VGA_WIDTH; x++)
		terminal_p.buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
		    ((__u16)terminal_p.color << 8) | ' ';

	/* Keep cursor on the last line */
	terminal_p.y = VGA_HEIGHT - 1;
	terminal_p.x = 0;
}

/*
 * Identify and treat symbols
 *
 * -> `bool`: Is symbol
 * @c: The symbol
 */
static inline bool _terminal_symbol(char c)
{
	switch (c)
	{
		case '\n':
		{
			terminal_p.x = 0;
			terminal_p.y++;

			if (terminal_p.y >= VGA_HEIGHT)
				_terminal_scroll();
			return true;
		}
	}
	return false;
}

/*
 * Write the framebuffer to VGA text buffer address
 *
 * -> `void`
 */
void __terminal_flush(void)
{
	char       *video = (char *)VGA_ADD;
	const char *msg   = (const char *)terminal_p.buffer;

	for (__u16 i = 0; i < VGA_BUF_SIZE; i++)
		video[i] = msg[i];
}
