#if CFG_CONSOLE_VGA
#include <rks/drivers/console/vga.h>
#endif

#include <rks/console.h>
#include <rks/math.h>
#include <rks/string.h>

struct console_ctx
{
	__u32 x, y;
	__u8  color;
};

static struct console_ctx console;
static inline void        handle_overflow (void);

/*
 * Initialize the virtual terminal
 *
 * -> `void`
 */
void
_terminal_init (void)
{
#if CFG_CONSOLE_VGA
	vga_init_context ();
#endif

#if CFG_CONSOLE_GOP
	// gop_init_context
#endif
}

/*
 * Write a character at the current terminal coordinate
 *
 * -> `void`
 * @c: The character
 */
void
_terminal_putchar (char **c)
{
	switch (**c)
	{
		case '\n':
		{
			console.x = 0;
			console.y++;
			handle_overflow ();
			return;
		}

		case '\\':
		{
			(*c)++;
			break;
		}

		case '>':
		{
			/* Treat colors modifiers (>[]) */
			if ((*c)[1] == '[')
			{
				/* Number character array length-only (within []) */
				size_t i = 0;
				while ((*c)[i + 2] != ']')
				{
					if ((*c)[i + 2] == '\0')
						return;
					i++;
				}

				if (!i)
					break;

				/* Convert this character array to number */
				console.color  = atoi (c[2], i);
				(*c)          += i + 2;
			}
			return;
		}

		case '{':
			return;

		case '}':
		{
#if CFG_CONSOLE_VGA
			console.color = VGA_DEFAULT_COLOR;
#endif
			return;
		}
	}

#if CFG_CONSOLE_VGA
	vga_put_char (**c, console.color, console.y * VGA_WIDTH + console.x);
#endif

	console.x++;
	handle_overflow ();
}

/*
 * Write a string into the framebuffer
 *
 * -> `void`
 * @c: Pointer to the string
 */
void
_terminal_write (char *c)
{
	for (; *c != '\0'; c++)
		_terminal_putchar (&c);
#if CFG_CONSOLE_VGA
	console.color = VGA_DEFAULT_COLOR;
#endif
}

void
handle_overflow (void)
{
#if CFG_CONSOLE_VGA
	if (console.x >= VGA_WIDTH)
	{
		console.x = 0;
		console.y++;
	}

	if (console.y >= VGA_HEIGHT)
		_terminal_scroll ();
#endif
}

/*
 * Scroll terminal to the next line
 *
 * -> `void`
 */
void
_terminal_scroll (void)
{
#if CFG_CONSOLE_VGA
	struct vga_buffer_char vga_b;

	/* Copy each row to its previous one */
	for (__u16 y = 1; y < VGA_HEIGHT; y++)
	{
		for (__u16 x = 0; x < VGA_WIDTH; x++)
		{
			vga_get_char (y * VGA_WIDTH + x, &vga_b);
			vga_put_char (vga_b.character, vga_b.color, (y - 1) * VGA_WIDTH + x);
		}
	}

	/* Clear the last row */
	for (__u16 x = 0; x < VGA_WIDTH; x++)
		vga_put_char (' ', VGA_DEFAULT_COLOR, (VGA_HEIGHT - 1) * VGA_WIDTH + x);

	/* Keep cursor on the last line */
	console.y = VGA_HEIGHT - 1;
	console.x = 0;
#endif
}

/*
 * Write the framebuffer to the VGA text buffer address
 *
 * -> `void`
 */
void
__terminal_flush (void)
{
#if CFG_CONSOLE_VGA
	vga_flush_buffer ();
#endif
}
