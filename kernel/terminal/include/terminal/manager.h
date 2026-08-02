#ifndef _KERNEL_TERMINAL_MANAGER_H
#define _KERNEL_TERMINAL_MANAGER_H

#include <stddef.h>
#include <stdint.h>

/* Virtual terminal global state */
typedef struct tty
{
	HISTORY* history;
} TTY;

/* Manager operation functions (layer after dispatch) */
struct manager_ops
{

	/* Put a new character */
	void ( *putc ) ( HISTORY *, char );

	/* Put a new string */
	void ( *puts ) ( HISTORY *, char*  );

	/* Clears all TTY history */
	void ( *clear ) ( HISTORY * );

	/* Renders all viewport dirty lines */
	void ( *render ) ( HISTORY * );
};

/* Operations */
kerr_t terminal_manager_init ( void );

#endif /* _KERNEL_TERMINAL_MANAGER_H */
