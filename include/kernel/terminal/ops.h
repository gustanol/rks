#ifndef _KERNEL_TERMINAL_OPS_H
#define _KERNEL_TERMINAL_OPS_H

#include "types.h"

void terminal_putc ( TTY id, char c );
void terminal_write ( TTY id, const char* buf, size_t len );
void terminal_puts ( TTY id, const char* str );

void terminal_clear ( TTY id );
void terminal_scroll ( TTY id, ssize_t lines );

#endif /* _KERNEL_TERMINAL_OPS_H */
