#ifndef _KERNEL_ALLOC_ALLOCATOR_HEAP_H
#define _KERNEL_ALLOC_ALLOCATOR_HEAP_H

#include <kernel/kerr/kerr.h>
#include <kernel/mem/vmm/types.h>
#include <stdbool.h>

/**
 * Heap allocator context creator
 * Allocator function
 *
 * Return -> `struct heap *`: Heap context pointer (opaque type only accessible through ALLOC subsystem)
 * Arguments:
 *   1. `void`
 * Prototype
 **/
#define HEAP_ALLOC_CREATE \
	struct heap* _heap_create ( void )

/**
 * Heap allocator context destroyer 
 * Allocator function
 *
 * Return -> `struct heap *`: Heap context pointer (opaque type only accessible through ALLOC subsystem)
 * Arguments:
 *   1. `struct heap* heap`: Heap object to be destroyed
 * Prototype
 **/
#define HEAP_ALLOC_DESTROY(name) \
	  kerr_t name ( struct heap* heap)

/**
 * Heap allocator memory allocator
 * Allocator function
 *
 * Return -> `void *`: Pointer to allocated object
 * Arguments:
 *   1. `struct heap* heap`: Heap state
 *   2. `size_t size`: Object size
 * Prototype
 **/
#define HEAP_ALLOC_MALLOC(name) \
	void* name ( struct heap* heap, size_t size )

/**
 * Heap allocator memory freer
 * Allocator function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void* ptr`: Payload base address
 * Prototype
 **/
#define HEAP_ALLOC_MFREE(name) \
	  kerr_t name ( void* payload )

/**
 * Heap area grower
 * Backend function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct heap* heap`: Heap to be grown
 *   2. `size_t frames_needed`: As it grows by frames, the minimum amount to be grown
 * Prototype
 **/
#define HEAP_ALLOC_GROW(name) \
	  kerr_t name ( struct heap* heap, size_t frames_needed )

/* Heap allocator operation functions */
struct heap_ops
{

	/* Context destroyer */
	HEAP_ALLOC_DESTROY ( ( *destroy ) );

	/* Memory allocator */
	HEAP_ALLOC_MALLOC ( ( *malloc ) );

	/* Memory freer */
	HEAP_ALLOC_MFREE ( ( *mfree ) );

	/* Heap grower */
	HEAP_ALLOC_GROW ( ( *grow ) );
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
	vaddr_t arena_start;        /* Base address */
	vaddr_t current_arena;      /* Current frame base address */
	size_t limit;               /* Limit size */
	bool ready;                 /* Is it ready to be used? */
	struct heap_ops* ops;       /* Operations */
};

/* Heap creator function prototype */
HEAP_ALLOC_CREATE;

#endif /* _KERNEL_ALLOC_ALLOCATOR_HEAP_H */
