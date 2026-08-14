#ifndef _KERNEL_TERMINAL_TTY_H
#define _KERNEL_TERMINAL_TTY_H

#include "types.h"

/* TTY-specific operations */
tty_id terminal_create_tty ( void );
kerr_t terminal_destroy_tty ( tty_id id );
kerr_t terminal_switch_tty ( tty_id id );
tty_id terminal_get_active_tty ( void );

#endif /* _KERNEL_TERMINAL_VIRTUAL_H */
