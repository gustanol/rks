#include <terminal/dispatch.h>
#include <terminal/internal.h>

static inline struct tty*
get_tty_by_id ( tty_id id )
{
	return ( id < count ) ? g_terminal->tty.list[id] : NULL;
}

tty_id
_terminal_dispatch_create_tty ( void )
{
	return g_terminal->tty.ops->create ();
}

kerr_t
_terminal_dispatch_destroy_tty ( tty_id id )
{
	return g_terminal->tty.ops->destroy ( id );
}

void
_terminal_dispatch_switch_tty ( tty_id id )
{
	g_terminal->tty.current = id;
}

tty_id
_terminal_dispatch_get_active_tty ( void )
{
	return g_terminal->tty.current;	
}

void
_terminal_dispatch_putc ( tty_id id, char c )
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->putc ( tty->history, c );
}

void
_terminal_dispatch_write ( tty_id id, const char* buf, size_t len )
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->write ( tty->history, buf, len );
}

void
_terminal_dispatch_puts ( tty_id id, const char* str )
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->puts ( tty->history, str );
}

void
_terminal_dispatch_clear ( tty_id id );
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->clear ( tty->history );
}

void
_terminal_dispatch_scroll ( tty_id id, int64_t lines )
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->scroll ( tty->history, lines );
}

void
_terminal_dispatch_render ( tty_id id )
{
	struct tty* tty = get_tty_by_id ( id );
	if ( tty == NULL )
		return;

	return g_terminal->tty.ops->render ( tty->history );
}
