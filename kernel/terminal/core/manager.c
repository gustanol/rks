#include <string.h>
#include <terminal/internal.h>
#include <terminal/manager.h>

/* PROBABLY, FIXED DIMENSIONS WON'T BE USED IN THE FUTURE */
#define TTY_CHUNK_LINES 64
#define TTY_LINE_CELLS 80

#define TTY_FOR_EACH_BACKEND(i, count) \
    for ( uint32_t i = 0; i < count; i++ )

/**
 * Color descriptor
 * They're gonna be stored with each cell
 *
 * Note it's gonna be interpreted individually by each backend,
 * Whose can decide implementing it or no
 **/
typedef struct
{

	/* Type RGB */
	uint8_t r,
	  g,
	  b;
} COLOR;

/* Cell style descriptor */
typedef struct
{
	COLOR fg;                   /* Foreground */
	COLOR bg;                   /* Background */
} STYLE;

/* Default cell style */
STYLE default_style = {

	/* *INDENT-OFF* */

	.fg = {
		.r = 255,
		.g = 255,
		.b = 255,
	},

	.bg = {
		.r = 0,
		.g = 0,
		.b = 0,
	},

	/* *INDENT-ON* */
};

/* Cell descriptor */
typedef struct
{
	STYLE style;
	char c;                     /* Character */
} CELL;

/* Line descriptor */
typedef struct
{
	struct
	{
		CELL base[TTY_LINE_CELLS];
		uint32_t current;
	} cells;

	/* Is this line dirty? */
	bool dirty;
} LINE;

/* Chunk of lines */
typedef struct
{
	struct
	{

		/* Array of lines descriptors */
		LINE base[TTY_CHUNK_LINES];

		/* Current available line */
		uint32_t current;
	} lines;
} CHUNK;

/* History containing all chunks */
typedef struct
{
	struct
	{

		/* Array of chunks */
		CHUNK** base;

		/* Count of allocated chunks */
		uint32_t count;

		/* Maximum chunks that can be allocated */
		uint32_t capability;
	} chunks;

	struct
	{
		uint32_t line_count;
		uint32_t viewport_top;
		uint32_t width;
		uint32_t height;
	} attributes;
} HISTORY;

/**
 * TTY chunk getter by line index
 * Internal inline function
 *
 * Return -> `CHUNK *`: Chunk base pointer
 * Arguments:
 *   1. `uint32_t line`: Line index
 *   2. `HISTORY* history`: TTY history
 **/
static inline CHUNK*
tty_get_chunk_by_line ( uint32_t line, HISTORY* history )
{

	/* Only returns if the index is valid */
	return ( line / TTY_CHUNK_LINES < history->chunks.count )
	    ? history->chunks.base[line / TTY_CHUNKS_LINES] : NULL;
}

/**
 * TTY viewport top chunk getter by viewport top line
 * Internal inline function
 *
 * Return -> `CHUNK *`: Chunk base pointer
 * Arguments:
 *   1. `uint32_t top`: Viewport top
 *   2. `HISTORY* history`: TTY history
 **/
static inline CHUNK*
tty_get_viewport_top_chunk ( uint32_t top, HISTORY* history )
{
	return tty_get_chunk_by_line ( top, history );
}

/**
 * TTY line offset within a chunk calculator
 * Internal inline function
 *
 * Return -> `LINE *`: Line base pointer
 * Arguments:
 *   1. `uint32_t line_count`: Total line count
 *   2. `CHUNK* chunk`: Chunk base pointer
 **/
static inline LINE*
tty_get_offset_line ( uint32_t line_count, CHUNK* chunk )
{
	return &chunk->lines.base[line_count % TTY_CHUNK_LINES];
}

/**
 * TTY current cell getter
 * Internal inline function
 *
 * Return -> `CELL *`
 * Arguments:
 *   1. `LINE* line`: Line base pointer
 **/
static inline CELL*
tty_get_current_cell ( LINE* line )
{
	return &line->cells.base[line->cells.current++];
}

/* Defines manager operations */
struct manager_ops ops = {
	.putc = _terminal_manager_putc,
	.puts = _terminal_manager_puts,
	.clear = _terminal_manager_clear,
};

/* Keep it as a const pointer */
struct manager_ops const* ops_ptr = &ops;

/**
 * Terminal manager initializer function
 * Manager function
 *
 * Return -> `kerr_t`
 * Arguments:
 *   - `void`
 * Callers:
 *   - `terminal_init`
 **/
kerr_t
terminal_manager_init ( void )
{

	/* ALLOCATE A NEW CHUNK WHEN THE FUNCTION IS READY */

	/* Set the operations to be used by higher layers */
	g_terminal->tty.manager_ops = ops_ptr;

	/* Everthing's okay */
	return KERR_OK;
}

/**
 * Terminal manager function to add a new character
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY chunk content history
 *   - `char c`: Character to be put in
 * Callers:
 *   - `_terminal_dispatch_putc`
 **/
void
_terminal_manager_putc ( HISTORY* history, char c )
{
	switch ( c )
	{

				/* Treat newlines */
			case '\n':
			{
				history->attributes.line_count++;
				_terminal_manager_handle_overflow ( history );
				return;
			}
	}

	/* ADD VERIFICATION FOR OVERFLOW AND ALLOCATE NEW CHUNKS WHEN NEEDED */

	/* Get the current chunk based on the global line count */
	CHUNK* current_chunk = tty_get_chunk_by_line ( history->attributes.line_count, history );

	/* Get the current line within the current chunk */
	LINE* current_line = tty_get_offset_line ( history->attributes.line_count,
	    current_chunk );

	/* Cell to write the character to */
	CELL* cell = tty_get_current_cell ( current_line );

	/* Write the character at the current line cell */
	cell->c = c;
	cell->style = default_style;
	current_line->dirty = true;
}

/**
 * Terminal manager function to add a new null-terminated string
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be modified
 *   - `char* s`: Null-terminated string to be added
 * Callers:
 *   - `_terminal_dispatch_puts`
 **/
void
_terminal_manager_puts ( HISTORY* history, char* s )
{

	/* PARSE SPECIAL CASES BEFORE CALLING `PUTC` */

	for ( uint32_t i = 0; s[i] != '\0'; i++ )
		_terminal_manager_putc ( s[i] );
}

/**
 * Terminal manager function to clear all TTY history
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be cleared up
 * Callers:
 *   - `_terminal_dispatch_clear`
 **/
void
_terminal_manager_clear ( HISTORY* history )
{

	/**
	 * Sets all chunks to zero
	 * By setting them as zero, they are cleared after all
	 *
	 * This can be made by an iteration for each allocated chunk
	 **/
	for ( uint32_t i = 0; i < history->chunks.count; i++ )

	    /* Frees this chunk */
		k_mfree ( history->chunks.base[i] );

	/* Frees the chunk array base pointer */
	k_mfree ( history->chunks.base );

	/* Set the current chunk count as zero */
	history->chunks.count = 0;

	/* Just like the current capability */
	history->chunks.capability = 0;
}

/**
 * Terminal manager function used to render content
 * Static manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be used
 * Callers:
 *   - Internal Terminal manager functions (static)
 **/
void
_terminal_manager_render ( HISTORY* history )
{

	uint32_t height = history->attributes.height;
	uint32_t top = history->attributes.viewport_top;
	CHUNK* top_chunk = tty_get_viewport_top_chunk ( top, history );

	/* Do nothing for NULL return */
	if ( top_chunk == NULL )
		return;

	/* Iterate up to height limit */
	for ( uint32_t remaining = height; remaining > 0; remaining--, top++ )
	{

		/**
		 * Current iteration line
		 *
		 * Note it can be obtained by the current value of `top`
		 * The index is obtained by making a mod operation
		 * (`top % TTY_CHUNK_LINES`), which will result on the offset
		 **/
		LINE* line = tty_get_offset_line ( top, top_chunk );

		/* If current line is dirty, render it */
		if ( line->dirty )
		{

			STYLE current_style = line->cells.base[0].style;
			uint32_t write_from = 0;

			/* Iterate for each line character */
			for ( uint32_t i = 1; i < TTY_LINE_CELLS; i++ )
			{

				/* We must write substrings with different styles independently */
				if ( current_style != line->cells.base[i].style )
				{

					/**
					 * As the current cell style is different,
					 * We must write the last substring `til this point
					 * And update the current style
					 **/

					/* Set `current_style` */
					TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
					    g_terminal->backends.ops_list[backend]->set_style ( current_style );

					/**
					 * Write the substring from `write_from` to `i`
					 * All characters in it has the same style
					 *
					 * `write_from` -> Current substring start
					 * `i` -> Current character. When style is different,
					 *   It's the end of current substring
					 **/
					TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
					    g_terminal->backends.ops_list[backend]->write_at ( write_from,
					    &line->cells[write_from], i - write_from );

					/* Update the current substring state */
					current_style = line->cells.base[i].style;
					write_from = i;
				}
			}

			/* `current_style` -> Current substring style */
			TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
			    g_terminal->backends.ops_list[backend]->set_style ( current_style );

			/* Write the rest of the line */
			TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
			    g_terminal->backends.ops_list[backend]->write_at ( write_from,
			    &line->cells[write_from], TTY_LINE_CELLS );

			/* All line written */
			line->dirty = false;
		}
	}

	/* Flush every backend buffer */
	TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
	    g_terminal->backends.ops_list[backend]->flush ( &buf, TTY_LINE_CELLS,
	    TTY_CHUNK_LINES * viewport_chunks_limit );
}
