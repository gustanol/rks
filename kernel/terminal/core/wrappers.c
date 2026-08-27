#include <kernel/kerr/kerr.h>
#include <kernel/terminal/ops.h>
#include <kernel/terminal/tty.h>
#include <kernel/terminal/types.h>
#include <terminal/dispatch.h>

/**
 * Public TTY creator
 * Public wrapper
 *
 * Return `tty_id`: TTY ID value
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
tty_id
terminal_create_tty ( void )
{
	return _terminal_dispatch_create_tty (  );
}

/**
 * Public TTY destroyer
 * Public wrapper
 *
 * Return `kerr_t`: Kernel global error type 
 * Arguments:
 *   1. `tty_id id`: TTY id
 * Callers:
 *   - Many
 **/
kerr_t
terminal_destroy_tty ( tty_id id )
{
	KERR_RET_IF ( IS_KERR ( id ), KERR_INVAL, "The given TTY ID is KERR\n" );
	return _terminal_dispatch_destroy_tty ( id );
}

/**
 * Public TTY switcher
 * Public wrapper
 *
 * Return `kerr_t`: Kernel global error type 
 * Arguments:
 *   1. `tty_id id`: TTY id to switch to
 * Callers:
 *   - Many
 **/
kerr_t
terminal_switch_tty ( tty_id id )
{
	KERR_RET_IF ( IS_KERR ( id ), KERR_INVAL, "The given TTY ID is KERR\n" );
	_terminal_dispatch_switch_tty ( id );

	return KERR_OK;
}

/**
 * Public active TTY getter
 * Public wrapper
 *
 * Return `tty_id`: TTY id
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
tty_id
terminal_get_active_tty ( void )
{
	return _terminal_dispatch_get_active_tty (  );
}

/**
 * Private character inserter
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `char c`: Character to be inserted
 * Callers:
 *   - Many
 **/
void
terminal_putc ( tty_id id, char c )
{
	_terminal_dispatch_putc ( id, c );
}

/**
 * Public buffer writer
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `const char* buf`: Buffer to be inserted
 *   3. `size_t len`: Buffer length
 * Callers:
 *   - Many
 **/
void
terminal_write ( tty_id id, const char* buf, size_t len )
{
	_terminal_dispatch_write ( id, buf, len );
}

/**
 * Public string writer
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be modified
 *   2. `const char* str`: Null-terminated string to be inserted 
 * Callers:
 *   - Many
 **/
void
terminal_puts ( tty_id id, const char* str )
{
	_terminal_dispatch_puts ( id, str );
}

/**
 * Public TTY history cleaner
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification
 * Callers:
 *   - Many
 **/
void
terminal_clear ( tty_id id )
{
	_terminal_dispatch_clear ( id );
}

/**
 * Public TTY scroller
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification
 *   2. `int64_t lines`: Lines to be scrolled
 *     - Can be negative to scroll back
 * Callers:
 *   - Many
 **/
void
terminal_scroll ( tty_id id, int64_t lines )
{
	_terminal_dispatch_scroll ( id, lines );
}

/**
 * Public TTY viewport renderer 
 * Public wrapper
 *
 * Return `void`
 * Arguments:
 *   1. `tty_id id`: TTY identification to be rendered
 * Callers:
 *   - Many
 **/
void
terminal_render ( tty_id id )
{
	_terminal_dispatch_render ( id );
}
