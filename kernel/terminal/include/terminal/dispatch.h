#ifndef _KERNEL_TERMINAL_DISPATCH_H
#define _KERNEL_TERMINAL_DISPATCH_H

#include <kernel/kerr/kerr.h>
#include <kernel/terminal/types.h>

/* TTY dispatch operations */
tty_id _terminal_dispatch_create_tty ( void );
kerr_t _terminal_dispatch_destroy_tty ( tty_id tty );
kerr_t _terminal_dispatch_switch_tty ( tty_id tty );
kerr_t _terminal_dispatch_get_active_tty ( void );

/* Main operations */
void _terminal_dispatch_putc ( tty_id id, char c );
void _terminal_dispatch_write ( tty_id id, const char* buf, size_t len );
void _terminal_dispatch_puts ( tty_id id, const char* str );
void _terminal_dispatch_clear ( tty_id id );
void _terminal_dispatch_scroll ( tty_id id, int64_t lines );
void _terminal_dispatch_render ( tty_id id );

#endif /* _KERNEL_TERMINAL_DISPATCH_H */
