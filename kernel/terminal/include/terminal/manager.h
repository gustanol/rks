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

	/* Creates a new TTY object */
	tty_id ( *create ) ( void );

	/* Destroys a TTY object */
	kerr_t ( *destroy ) ( tty_id );

	/* Write a new sized-string */
	void ( *write ) ( HISTORY *, char* , size_t );

	/* Put a new character */
	void ( *putc ) ( HISTORY *, char );

	/* Put a new string */
	void ( *puts ) ( HISTORY *, char*  );

	/* Clears all TTY history */
	void ( *clear ) ( HISTORY * );

	/* Scrolls the history content */
	void ( *scroll ) ( HISTORY *, int64_t );

	/* Renders all viewport dirty lines */
	void ( *render ) ( HISTORY * );
};

/* Operations */
kerr_t terminal_manager_init ( void );

#endif /* _KERNEL_TERMINAL_MANAGER_H */
