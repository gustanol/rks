#ifndef _KERNEL_TERMINAL_INTERNAL_H
#define _KERNEL_TERMINAL_INTERNAL_H

#include "backend.h"
#include <stddef.h>
#include <stdint.h>

struct terminal_ctx
{
	struct
	{
		struct terminal_ops** ops_list;
		size_t count;
	} backends;

	bool ready;
};

/* Main pointer */
extern struct terminal_ctx const* g_terminal;

#endif /* _KERNEL_terminal_INTERNAL_H */
