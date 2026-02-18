#include <rks/drivers/console/vga.h>
#include <rks/console/console.h>
#include <rks/console/dispatcher.h>
#include <rks/math.h>
#include <rks/string.h>

/* Console context struct */
struct console_ctx
{
	struct
	{
		__u32 x,
		  y;
	} coordinates;

	struct
	{
		__u8 color;
		size_t width;
		size_t height;
	} attributes;

	struct screen_cell* screen;
};

/*
 * TODO: Allocate this buffer dynamically on init and on every resize
 * It'd be useful for bigger framebuffers supported for other kind of drivers but VGA
 *
 * When resized, smaller buffers will not be altered
 */
static struct screen_cell screen[SCREEN_BUFFER_SIZE];
static struct console_ctx console;

/* Console drivers specifics */
static struct console_driver* consoles[MAX_CONSOLES];
static __u32 console_count = 0;

/* Prototypes */
static inline void handle_overflow (void);
static void console_scroll (void);

/*
 * Initialize the virtual console
 *
 * -> `void`
 */
void
console_init (void)
{
	/* Start console context */
	console.coordinates.x = console.coordinates.y = 0;
	console.attributes.color = (__u8) CONSOLE_DEFAULT_COLOR;
	console.attributes.width = (size_t) SCREEN_WIDTH;
	console.attributes.height = (size_t) SCREEN_HEIGHT;
	console.screen = screen;

#if CFG_CONSOLE_VGA
	vga_init_context ();
#endif
}

/*
 * Console drivers register
 * Only used by drivers to register themselves
 *
 * -> void
 * @drv: Pointer to console driver identifier struct
 */
void
console_register (struct console_driver* drv)
{
	if (console_count < MAX_CONSOLES)
		consoles[console_count++] = drv;
}

/*
 * Write a character at the current terminal coordinate
 *
 * -> `void`
 * @c: The character
 */
void
console_putchar (char** c)
{
	switch (**c)
	{
			/* Newline */
		case '\n':
			{
				console.coordinates.x = 0;
				console.coordinates.y++;
				handle_overflow ();
				return;
			}

			/*
			 * Backslash
			 *
			 * It can be used to print control characters, such as `{`
			 */
		case '\\':
			{
				(*c)++;
				break;
			}

			/* Colors */
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
					console.attributes.color =
						atoi (&(*c)[2], i);
					(*c) += i + 2;
				}
				return;
			}

		case '{':
			return;

		case '}':
			{
				console.attributes.color =
					CONSOLE_DEFAULT_COLOR;
				return;
			}
	}

	/*
	 * Copy the current character and its color attribute to the current screen cell
	 *
	 * NOTE: For each height layer, there were be one completed row
	 * That's the reason we must use `y * width + x`
	 */
	console.screen[console.coordinates.y * console.attributes.width +
		       console.coordinates.x].character = (__u8) ** c;
	console.screen[console.coordinates.y * console.attributes.width +
		       console.coordinates.x].color =
		console.attributes.color;

	/* Advance the line and treat overflow */
	console.coordinates.x++;
	handle_overflow ();
}

/*
 * Write a string into the framebuffer
 *
 * -> void
 * @c: Pointer to the string
 */
void
console_write (char* c)
{
	for (; *c != '\0'; c++)
		console_putchar (&c);
	console.attributes.color = CONSOLE_DEFAULT_COLOR;
}

/*
 * Screen overflow handler
 *
 * -> void
 */
void
handle_overflow (void)
{
	if (console.coordinates.x >= SCREEN_WIDTH)
	{
		console.coordinates.x = 0;
		console.coordinates.y++;
	}

	if (console.coordinates.y >= SCREEN_HEIGHT)
		console_scroll ();
}

/*
 * Console scroller
 *
 * -> `void`
 */
void
console_scroll (void)
{
	/* Copy each row to the previous one */
	for (__u16 y = 1; y < console.attributes.height; y++)
		for (__u16 x = 0; x < console.attributes.width; x++)
			console.screen[(y - 1) * console.attributes.width +
				       x] =
				console.screen[y * console.attributes.width +
					       x];

	/* Clear the last row */
	for (__u16 x = 0; x < console.attributes.width; x++)
	{
		console.screen[(console.attributes.height -
				1) * console.attributes.width + x].character =
			(__u8) ' ';
		console.screen[(console.attributes.height -
				1) * console.attributes.width + x].color =
			(__u16) CONSOLE_DEFAULT_COLOR;
	}

	/* Keep cursor on the last line */
	console.coordinates.y = console.attributes.height - 1;
	console.coordinates.x = 0;
}

/*
 * Write the framebuffer to the VGA text buffer address
 *
 * -> `void`
 */
void
console_flush (void)
{
	/* Flush the current screen to every registered driver */
	for (__u32 i = 0; i < console_count; i++)
		consoles[i]->flush (console.screen);
}
