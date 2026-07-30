#include <terminal/internal.h>

/*
kerr_t
terminal_manager_init ( void )
{
}
*/

void
_terminal_manager_putc ( struct tty_history* history, char c )
{
	switch ( c )
	{
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
