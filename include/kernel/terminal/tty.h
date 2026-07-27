#ifndef _KERNEL_TERMINAL_TTY_H
#define _KERNEL_TERMINAL_TTY_H

#include "types.h"

/* TTY-specific operations */
TTY terminal_create_tty ( void );
kerr_t terminal_destroy_tty ( TTY id );
kerr_t terminal_switch_tty ( TTY id );
kerr_t terminal_get_active_tty ( void );

#endif /* _KERNEL_TERMINAL_VIRTUAL_H */
