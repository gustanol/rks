#ifndef _KERNEL_TERMINAL_MANAGER_H
#define _KERNEL_TERMINAL_MANAGER_H

#include <stddef.h>
#include <stdint.h>

#define TTY_CHUNK_LINES 64
#define TTY_GET_CHUNK_IDX(LINE_COUNT) \
        ( ( LINE_COUNT ) / ( TTY_CHUNK_LINES ) )

/* Line descriptor */
struct tty_line
{
	struct
	{
		char* base;
		size_t count;
		size_t current;
	} cells;
};

/* Chunk of lines */
struct tty_chunk
{
	struct
	{

		/* Array of lines descriptors */
		struct tty_line base[TTY_CHUNK_LINES];

		/* Amount of used lines */
		size_t used;
	} lines;
};

/* History containing all chunks */
struct tty_history
{
	struct
	{

		/* Array of chunks */
		struct tty_chunk** base;

		/* Count of allocated chunks */
		size_t count;

		/* Maximum chunks that can be allocated */
		size_t capability;
	} chunks;

	struct
	{
		size_t line_count;
		size_t width;
		size_t height;
	} attributes;
};

/* Virtual terminal global state */
struct tty
{
	struct tty_history* history;
	size_t viewport_top;
};

/* Manager operation functions (layer after dispatch) */
struct manager_ops
{

	/* Put new character */
	void ( *putc ) ( struct tty_history *, char );

	/* Put new string */
	void ( *puts ) ( struct tty_history *, char * );

	/* Clears all TTY history */
	void ( *clear ) ( struct tty_history * );
};

/* Operations */
kerr_t terminal_manager_init ( void );

#endif /* _KERNEL_TERMINAL_MANAGER_H */
