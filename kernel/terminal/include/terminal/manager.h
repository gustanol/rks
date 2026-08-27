#ifndef _KERNEL_TERMINAL_MANAGER_H
#define _KERNEL_TERMINAL_MANAGER_H

#include <kernel/terminal/types.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Terminal manager initializer function
 * Manager function
 *
 * Return -> `kerr_t`: Global kernel error type
 * Arguments:
 *   - `void`
 * Prototype
 **/
#define TERMINAL_MANAGER_INIT \
	kerr_t terminal_manager_init ( void )

/**
 * Terminal manager function used to create a new TTY object
 * Manager function
 *
 * Return -> `tty_id`: TTY index or `kerr_t`
 * Arguments:
 *   1. `void`
 * Prototype
 **/
#define TERMINAL_MANAGER_CREATE(name) \
	tty_id name ( void )

/**
 * Terminal manager function used to destroy a TTY history
 * Manager function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `tty_id id`: TTY ID to be destroyed
 * Prototype
 **/
#define TERMINAL_MANAGER_DESTROY(name) \
	kerr_t name ( tty_id id )

/**
 * Terminal manager function to add a new string buffer by its length
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be modified
 *   - `const char* buf`: String buffer to be added
 *   - `size_t len`: Buffer length
 * Prototype
 **/
#define TERMINAL_MANAGER_WRITE(name) \
	void name ( HISTORY* history, const char* buf, size_t len )

/**
 * Terminal manager function to add a new null-terminated string
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be modified
 *   - `const char* s`: Null-terminated string to be added
 * Prototype
 **/
#define TERMINAL_MANAGER_PUTS(name) \
	void name ( HISTORY* history, const char* s );

/**
 * Terminal manager function to add a new character
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY chunk content history
 *   - `const char c`: Character to be put in
 * Prototype
 **/
#define TERMINAL_MANAGER_PUTC(name) \
	void name ( HISTORY* history, const char c )

/**
 * Terminal manager function to clear all TTY history
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   - `HISTORY* history`: TTY history to be cleared up
 * Prototype
 **/
#define TERMINAL_MANAGER_CLEAR(name) \
	void name ( HISTORY* history )

/**
 * Terminal manager function used to scroll the screen content
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   1. `HISTORY* history`: TTY history to be scrolled
 *   2. `int64_t lines`: Amount of lines to be scrolled
 *     - It should serve either as a 32-bit positive or negative value
 * Prototype
 **/
#define TERMINAL_MANAGER_SCROLL(name) \
	void name ( HISTORY* history, int64_t lines )

/**
 * Terminal manager function used to render content
 * Manager function
 *
 * Return -> `void`
 * Arguments:
 *   1. `HISTORY* history`: TTY history to be used
 * Prototype
 **/
#define TERMINAL_MANAGER_RENDER(name) \
	void name ( HISTORY* history )

/* Opaque history type (only visible by the manager) */
typedef struct history HISTORY;

/* Virtual terminal global state */
typedef struct tty
{
	HISTORY* history;
} TTY;

/* Manager operation functions (layer after dispatch) */
struct manager_ops
{

	/* Creates a new TTY object */
	TERMINAL_MANAGER_CREATE ( ( *create ) );

	/* Destroys a TTY object */
	TERMINAL_MANAGER_DESTROY ( ( *destroy ) );

	/* Write a new sized-string */
	TERMINAL_MANAGER_WRITE ( ( *write ) );

	/* Put a new character */
	TERMINAL_MANAGER_PUTC ( ( *putc ) );

	/* Put a new string */
	TERMINAL_MANAGER_PUTS ( ( *puts ) );

	/* Clears all TTY history */
	TERMINAL_MANAGER_CLEAR ( ( *clear ) );

	/* Scrolls the history content */
	TERMINAL_MANAGER_SCROLL ( ( *scroll ) );

	/* Renders all viewport dirty lines */
	TERMINAL_MANAGER_RENDER ( ( *render ) );
};

/* Operations */
TERMINAL_MANAGER_INIT;

#endif /* _KERNEL_TERMINAL_MANAGER_H */
