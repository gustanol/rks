#ifndef _KERNEL_TERMINAL_INTERNAL_H
#define _KERNEL_TERMINAL_INTERNAL_H

#include "backend.h"
#include "manager.h"
#include <stddef.h>
#include <stdint.h>

/* Terminal global state */
struct terminal
{
	struct
	{

		/* Operations */
		struct terminal_ops** ops_list;

		/* Array length */
		size_t count;
	} backends;

	struct
	{

		/* Array of TTY */
		struct tty* list;

		/* Array length */
		size_t count;

		/* Internal manager operations */
		struct manager_ops* ops;
	} tty;

	/* Is it ready? */
	bool ready;
};

/* Main pointer */
extern struct terminal const* g_terminal;

#endif /* _KERNEL_TERMINAL_INTERNAL_H */
