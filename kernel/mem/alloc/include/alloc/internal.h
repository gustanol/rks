#ifndef _KERNEL_ALLOC_INTERNAL_H
#define _KERNEL_ALLOC_INTERNAL_H

#include <alloc/allocators/heap.h>
#include <stdbool.h>

/* Allocaton context structure */
struct alloc_ctx
{
	/* Is it ready to use? */
	bool ready;

	/* Kernel state */
	struct
	{
		struct heap* heap;
		/* struct slab* slab_ctx; */
	} kernel;
};

/* Allocation global state pointer */
extern struct alloc_ctx* g_alloc;

#endif /* _KERNEL_ALLOC_INTERNAL_H */
