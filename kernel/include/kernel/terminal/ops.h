#ifndef _KERNEL_TERMINAL_OPS_H
#define _KERNEL_TERMINAL_OPS_H

#include "types.h"

void terminal_putc ( tty_id id, const char c );
void terminal_write ( tty_id id, const char* buf, size_t len );
void terminal_puts ( tty_id id, const char* s );
void terminal_clear ( tty_id id );
void terminal_scroll ( tty_id id, int64_t lines );
void terminal_render ( tty_id id );

#endif /* _KERNEL_TERMINAL_OPS_H */
