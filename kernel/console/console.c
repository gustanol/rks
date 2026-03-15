#include <rks/kernel/console/console.h>
#include <rks/kernel/console/dispatcher.h>
#include <rks/lib/math.h>
#include <rks/lib/string.h>

/* Console context struct */
struct console_ctx
{
	struct
	{
		u32 x,
		  y;
	} coordinates;

	struct
	{
		u8 color;
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
static u32 console_count = 0;

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
	console.attributes.color = (u8) CONSOLE_DEFAULT_COLOR;
	console.attributes.width = (size_t) SCREEN_WIDTH;
	console.attributes.height = (size_t) SCREEN_HEIGHT;
	console.screen = screen;
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
					return;
				}
				break;
			}

			/*
			 * Special character
			 *
			 * Determine the start of color modifier
			 * Every text inside it will be applied
			 *
			 * It can be printed using `\\{`
			 */
		case '{':
			return;

			/*
			 * End of color modifier
			 */
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
		       console.coordinates.x].character = (u8) ** c;
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
	/*
	 * If we reach the end of the line, it means we must break to the next one
	 *
	 * For that reason, we increase `y` and set `x` to zero (start of the next line)
	 */
	if (console.coordinates.x >= SCREEN_WIDTH)
	{
		console.coordinates.x = 0;
		console.coordinates.y++;
	}

	/*
	 * Notice that, if we were already at the last row, we must scroll all lines
	 *
	 * In other hand, if we don't do it, there'd be an overflow of text
	 */
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
	for (u16 y = 1; y < console.attributes.height; y++)
		for (u16 x = 0; x < console.attributes.width; x++)
			console.screen[(y - 1) * console.attributes.width +
				       x] =
				console.screen[y * console.attributes.width +
					       x];

	/* Clear the last row */
	for (u16 x = 0; x < console.attributes.width; x++)
	{
		/* Null character */
		console.screen[(console.attributes.height -
				1) * console.attributes.width + x].character =
			(u8) ' ';

		/* Black background */
		console.screen[(console.attributes.height -
				1) * console.attributes.width + x].color =
			(u16) CONSOLE_DEFAULT_COLOR;
	}

	/* Keep cursor on the last line */
	console.coordinates.y = console.attributes.height - 1;
	console.coordinates.x = 0;
}

/*
 * Caller of every registered console driver flush function
 *
 * -> `void`
 */
void
console_flush (void)
{
	/* Flush the current screen to every registered driver */
	for (u32 i = 0; i < console_count; i++)
		consoles[i]->flush (console.screen);	/* Pass the screen framebuffer */
}
