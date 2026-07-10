#ifndef _KERNEL_ALLOC_INTERNAL_H
#define _KERNEL_ALLOC_INTERNAL_H

#include "backend.h"
#include <rks/kernel/types.h>

/* Allocaton context structure */
struct alloc_ctx
{
	int ready;
	struct alloc_ops* ops;
};

/* Allocation global state pointer */
extern struct alloc_ctx* g_alloc;

#endif /* _KERNEL_ALLOC_INTERNAL_H */
