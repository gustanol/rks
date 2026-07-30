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
	 	uint32_t count;
		uint32_t current;
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
		uint32_t used;
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
		uint32_t count;

		/* Maximum chunks that can be allocated */
		uint32_t capability;
	} chunks;

	struct
	{
		uint32_t line_count;
		uint32_t width;
		uint32_t height;
	} attributes;
};

/* Virtual terminal global state */
struct tty
{
	struct tty_history* history;
	uint32_t viewport_top;
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
