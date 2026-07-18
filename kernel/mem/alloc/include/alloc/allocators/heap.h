#ifndef _KERNEL_ALLOC_BACKEND_HEAP_H
#define _KERNEL_ALLOC_BACKEND_HEAP_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/vmm/types.h>

/* Heap allocator operation functions */
struct heap_ops
{
	/* Context creator */
	struct heap* ( *create ) ( void );

	/* Context destroyer */
	  kerr_t ( *destroy ) ( struct heap * );

	/* Memory allocator */
	void* ( *malloc ) ( struct heap *, size_t );

	/* Memory freer */
	  kerr_t ( *mfree ) ( void*  );

	/* Heap grower */
	  kerr_t ( *grow ) ( struct heap *, size_t );
};

/**
 * Heap context data
 *
 * `HEAP` is an opaque type
 * Its type is defined right here
 * It can only be accessed inside the ALLOC subsystem
 **/
struct heap
{
	vaddr_t arena_start;    /* Base address */
	vaddr_t current_arena;  /* Current frame base address */
	size_t limit;           /* Limit size */
	bool ready;             /* Is it ready to be used? */
	struct heap_ops* ops;   /* Operations */
};

/* Heap creator function prototype */
struct heap* _heap_create ( void );

#endif /* _KERNEL_ALLOC_BACKEND_HEAP_H */
