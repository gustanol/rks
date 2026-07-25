#ifndef _KERNEL_CONSOLE_INTERNAL_H
#define _KERNEL_CONSOLE_INTERNAL_H

#include "backend.h"
#include <stddef.h>
#include <stdint.h>

struct console
{
	struct
	{
		struct console_ops** ops_list;
		size_t count;
	} backends;

	bool ready;
};

/* Main pointer */
extern struct console const* g_console;

#endif /* _KERNEL_CONSOLE_INTERNAL_H */
