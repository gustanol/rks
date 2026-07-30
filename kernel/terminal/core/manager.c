#include <string.h>
#include <terminal/internal.h>

/* Defines manager operations */
struct manager_ops ops
{
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
 *   - `struct tty_history* history`: TTY chunk content history
 *   - `char c`: Character to be put in
 * Callers:
 *   - `_terminal_dispatch_putc`
 **/
void
_terminal_manager_putc ( struct tty_history* history, char c )
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
	struct tty_chunk* current_chunk =
	        history->
	        chunks.base[TTY_GET_CHUNK_IDX ( history->attributes.line_count )];

	/* Get the current line within the current chunk */
	struct tty_line* current_line =
	        &current_chunk->lines.base[history->attributes.line_count %
	                                   TTY_CHUNK_LINES];

	/* Write the character at the current line cell */
	current_line->cells.base[current_chunk->linex.cells.current++] = c;
}

/**
 * Terminal manager function to add a new null-terminated string
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `struct tty_history* history`: TTY history to be modified
 *   - `char* c`: Null-terminated string to be added
 * Callers:
 *   - `_terminal_dispatch_puts`
 **/
void
_terminal_manager_puts ( struct tty_history* history, char* s )
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
 *   - `struct tty_history* history`: TTY history to be cleared up
 * Callers:
 *   - `_terminal_dispatch_clear`
 **/
void
_terminal_manager_clear ( struct tty_history* history )
{

	/**
	 * Sets all chunks to zero
	 * By setting them as zero, they are cleared after all
	 *
	 * This can be made by an iteration for each allocated chunk
	 **/
	for ( uint32_t i = 0; i < history->chunks.count; i++ )

		/* Zero the `i` chunk, including all their lines */
		memset ( history->chunks.base[i], 0, sizeof ( struct tty_chunk ) );

	/**
	 * Zero the chunk base pointer at TTY history context
	 *
	 * Note it's an array of `struct tty_chunk *`
	 * Zero all its size
	 **/
	memset ( history->chunks.base, 0,
		sizeof ( struct tty_chunk* ) * history->chunks.capability );

	/* Set the current chunk count as zero */
	history->chunks.count = 0;

	/* Just like the current capability */
	history->chunks.capability = 0;
}
