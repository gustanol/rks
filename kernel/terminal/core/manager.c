#include <string.h>
#include <terminal/internal.h>
#include <terminal/manager.h>

/* PROBABLY, FIXED DIMENSIONS WON'T BE USED IN THE FUTURE */
#define TTY_CHUNK_LINES 64
#define TTY_LINE_CELLS 80

#define TTY_FOR_EACH_BACKEND(i, count) \
    for ( uint32_t i = 0; i < count; i++ )

#define CAPABILITY_INCREASE 8

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
	return ( line / history->attributes.height < history->chunks.count )
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
 *   2. `uint32_t height`: TTY height
 *   3. `CHUNK* chunk`: Chunk base pointer
 **/
static inline LINE*
tty_get_offset_line ( uint32_t line_count, uint32_t height, CHUNK* chunk )
{
	return &chunk->lines.base[line_count % height];
}

/**
 * TTY current cell getter
 * Internal inline function
 *
 * Return -> `CELL *`
 * Arguments:
 *   1. `LINE* line`: Line base pointer
 *   2. `uint32_t width`: TTY width
 **/
static inline CELL*
tty_get_current_cell ( LINE* line, uint32_t width )
{
	return ( line->cells.current < width ) ? &line->cells.base[line->cells.current++] : NULL;
}

/* Defines manager operations */
static const struct manager_ops ops = {
	.putc = _terminal_manager_putc,
	.write = _terminal_manager_write,
	.puts = _terminal_manager_puts,
	.clear = _terminal_manager_clear,
	.render = _terminal_manager_render,
	.create = _terminal_manager_create,
	/* .scroll */
};

/* Static prototypes */
static CHUNK* _terminal_manager_alloc_chunk ( HISTORY * history );

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

	/* Tries to create a new TTY */
	tty_id tty = _terminal_manager_create (  );
	KERR_RET_IF ( tty < 0, KERR_INTERNAL, "Couldn't create a default TTY" );

	/* Firstly, set this TTY as kernel default's */
	g_terminal->tty.current = tty;

	/* Set the operations to be used by higher layers */
	struct manager_ops const* ops_ptr = &ops;   /* Constant pointer */
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

	CHUNK* chunk;
	LINE* line;
	CELL* cell;

	uint32_t line_count = history->attributes.line_count;
	uint32_t height = history->attributes.height;

	/* Get the current chunk based on the global line count */
	chunk = tty_get_chunk_by_line ( line_count, history );
	if ( chunk == NULL )        /* It's problably gonna be used only for the first put */
	{
		chunk = _terminal_manager_alloc_chunk ( history );
		if ( chunk == NULL )
			return;
	}

	/* Get the current line within the current chunk */
	line = tty_get_offset_line ( line_count, height, chunk );

	/* Cell to write the character to */
	cell = tty_get_current_cell ( line );

	/* If `cell == NULL`, the line is full */
	if ( cell == NULL )
	{

		/* Advances the line count */
		line_count++;

		/**
         * Is it the last line in the chunk?
         *
         * `history->attributes.line_count % height == 1`
         *   - The count was already increased
         *   - The previous line would be zero because it's a multiple of height
         **/
		if ( line_count % height == 1 )
		{
			chunk = _terminal_manager_alloc_chunk ( history );

			/* Do nothing if it wasn't allocated */
			if ( chunk == NULL )
				return;
		}

		/* Update pointers */
		line = tty_get_offset_line ( line_count, height, chunk );
		cell = tty_get_current_cell ( line );
	}

	/* Write the character and its style after all */
	cell->c = c;
	cell->style = default_style;    // CHANGE DEFAULT STYLE
	line->dirty = true;

	/* Update counters */
	history->attributes.line_count = line_count;
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
	uint32_t width = history->attributes.width;
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
		 * (`top % height`), which will result on the offset
		 **/
		LINE* line = tty_get_offset_line ( top, top_chunk );

		/* If current line is dirty, render it */
		if ( line->dirty )
		{

			STYLE current_style = line->cells.base[0].style;
			uint32_t write_from = 0;

			/* Iterate for each line character */
			for ( uint32_t i = 1; i < width; i++ )
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
			    &line->cells[write_from], width );

			/* All line written */
			line->dirty = false;
		}
	}

	/* Flush every backend buffer */
	TTY_FOR_EACH_BACKEND ( backend, g_terminal->backends.count )
	    g_terminal->backends.ops_list[backend]->flush (  );
}

/**
 * Terminal manager function used to allocate new chunks
 * Static manager function
 *
 * Return -> `CHUNK *`: Chunk base pointer or NULL
 * Arguments:
 *   - `HISTORY* history`: TTY history to be used
 * Callers:
 *   - Internal Terminal manager functions (static)
 **/
CHUNK*
_terminal_manager_alloc_chunk ( HISTORY* history )
{

	CHUNK* chunk;
	uint32_t chunk_count = history->chunks.count;
	uint32_t chunk_capability = history->chunks.capability;

	/* If `count == capability`, all pointers are in use */
	if ( chunk_count == chunk_capability )
	{

		/* Store the old capability before updating it */
		uint32_t old_capability = chunk_capability;
		chunk_capability += CAPABILITY_INCREASE;

		/* Tries to allocate a new array of pointers */
		CHUNK** new_base = k_calloc ( sizeof ( CHUNK * ) * ( chunk_capability ) );
		if ( IS_KERR ( new_base ) )
			return NULL;

		/* Copy all existent chunk pointers */
		for ( uint32_t i = 0; i < old_capability; i++ )
			new_base[i] = history->chunks.base[i];

		/* Update the pointer */
		history->chunks.base = new_base;

		/* Frees the old array */
		k_mfree ( base_cpy );

		/* Update counters */
		history->chunks.count = chunk_count;
		history->chunks.capability = chunk_capability;
	}

	/* Allocate a new chunk */
	chunk = k_calloc ( sizeof ( CHUNK ) );
	if ( IS_KERR ( chunk ) )
		return NULL;

	/* Set the chunk */
	history->chunks.base[history->chunk.count++] = chunk;
}

/**
 * Terminal manager function used to create a new TTY object
 * Manager function
 *
 * Return -> `tty_id`: TTY index or -1 on errors
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `_terminal_dispatch_create`
 **/
tty_id
_terminal_manager_create ( void )
{

	TTY* tty;
	HISTORY* history;
	uint32_t tty_count = g_terminal->tty.count;
	uint32_t tty_capability = g_terminal->tty.capability;

	/* Allocate a new TTY object */
	tty = ( TTY * ) k_malloc ( sizeof ( TTY ) );
	if ( IS_KERR ( tty ) )
		goto err_ret;

	/* Allocate a new history for this TTY object */
	history = ( HISTORY * ) k_calloc ( sizeof ( HISTORY ) );
	if ( IS_KERR ( history ) )
		goto err_ret;

	/* Set this history */
	tty->history = history;
	history->attributes.width = TTY_LINE_CELLS;
	history->attributes.height = TTY_CHUNK_LINES;

	/* If `count` and `capability` are equal, no more pointers are available */
	if ( tty_count == tty_capability )
	{

		/* Make a copy of old capability and update its value */
		uint32_t old_capability = tty_capability;
		tty_capability += CAPABILITY_INCREASE;

		/* Tries to allocate a new array of TTY pointers */
		TTY* new_list = k_calloc ( sizeof ( TTY ) * ( tty_capability ) );
		if ( IS_KERR ( new_list ) )
			goto err_ret;

		/* From the old pointer, copy all already-allocated TTY objects */
		for ( uint32_t i = 0; i < old_capability; i++ )
			new_list[i] = g_terminal->tty.list[i];

		/* Update the pointer */
		g_terminal->tty.list = new_list;

		/* Frees the old array */
		k_mfree ( base_cpy );
	}

	/* Set the TTY pointer and counters */
	g_terminal->tty.list[tty_count++] = tty;
	g_terminal->tty.count = tty_count;
	g_terminal->tty.capability = tty_capability;

	/* Return the TTY index */
	return tty_count;

	/* Label used to clean allocated objects and return -1 (not created) */
  err_ret:

	if ( tty != NULL && !IS_KERR ( tty ) )
		k_mfree ( tty );

	if ( history != NULL && !IS_KERR ( history ) )
		k_mfree ( history );

	return -1;
}
