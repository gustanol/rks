#include <terminal/dispatch.h>
#include <terminal/internal.h>

/**
 * Private TTY creator
 * Private dispatcher
 *
 * Return `tty_id`: TTY ID value
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `terminal_create_tty`
 **/
tty_id
_terminal_dispatch_create_tty ( void )
{
	return g_terminal->tty.ops->create (  );
}

/**
 * Private TTY destroyer
 * Private dispatcher
 *
 * Return `kerr_t`: Kernel global error type 
 * Arguments:
 *   1. `tty_id id`: TTY id
 * Callers:
 *   - `terminal_destroy_tty`
 **/
kerr_t
_terminal_dispatch_destroy_tty ( tty_id id )
{
	return g_terminal->tty.ops->destroy ( id );
}

/**
 * Private TTY switcher
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY id to switch to
 * Callers:
 *   - `terminal_switch_tty`
 **/
void
_terminal_dispatch_switch_tty ( tty_id id )
{
	g_terminal->tty.current = id;
}

/**
 * Private helper used to get the current TTY in use
 * Private dispatcher
 *
 * Return `tty_id`: TTY id
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `terminal_get_active_tty`
 **/
tty_id
_terminal_dispatch_get_active_tty ( void )
{
	return g_terminal->tty.current;
}

/**
 * Private character inserter
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `char c`: Character to be inserted
 * Callers:
 *   - `terminal_putc`
 **/
void
_terminal_dispatch_putc ( tty_id id, char c )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->putc ( tty->history, c );
}

/**
 * Private buffer writer
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `const char* buf`: Buffer to be inserted
 *   3. `size_t len`: Buffer length
 * Callers:
 *   - `terminal_write`
 **/
void
_terminal_dispatch_write ( tty_id id, const char* buf, size_t len )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->write ( tty->history, buf, len );
}

/**
 * Private string writer
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `const char* str`: Null-terminated string to be inserted 
 * Callers:
 *   - `terminal_puts`
 **/
void
_terminal_dispatch_puts ( tty_id id, const char* str )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->puts ( tty->history, str );
}

/**
 * Private TTY history cleaner
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification
 * Callers:
 *   - `terminal_clear`
 **/
void
_terminal_dispatch_clear ( tty_id id )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->clear ( tty->history );
}

/**
 * Private TTY scroller
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification
 *   2. `int64_t lines`: Lines to be scrolled
 *     - Can be negative to scroll back
 * Callers:
 *   - `terminal_scroll`
 **/
void
_terminal_dispatch_scroll ( tty_id id, int64_t lines )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->scroll ( tty->history, lines );
}

/**
 * Private TTY visible content renderer
 * Private dispatcher
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification
 * Callers:
 *   - `terminal_render`
 **/
void
_terminal_dispatch_render ( tty_id id )
{

	if ( id >= g_terminal->tty.count )
		return;

	struct tty* tty = g_terminal->tty.list[id];
	return g_terminal->tty.ops->render ( tty->history );
}
