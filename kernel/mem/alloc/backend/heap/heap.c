#include "heap.h"
#include <alloc/backend.h>
#include <rks/kernel/mem/pmm.h>

/* Operations */
const struct alloc_ops ops = {
	.malloc = _heap_malloc,
	.free = _heap_free,
};

/* Backend context */
const struct alloc_backend backend = {

        /* *INDENT-OFF* */

	.base = {
		 .name = "heap_alloc_backend",
		 .tag = SUBSYS_TAG_ALLOC,
	},

        /* *INDENT-ON* */

	.type = ALLOC_BACKEND_HEAP,
	.init = __heap_init,
	.ops = &ops,
};

/* Register this backend */
REGISTER_ALLOC_BACKEND ( backend );

/**
 * Heap backend initializer
 * Backend function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct heap* heap`: Kernel heap context
 * Callers:
 *   - `alloc_init`
 **/
kerr_t
__heap_init ( struct heap* heap )
{
	/* Duplicate calling */
	if ( heap->ready )
		return KERR_REXIST;

	/* HEAP: Size and physical allocation */
	size_t frames_needed = FRAMES_BY_SIZE ( HEAP_INITIAL_SIZE );
	paddr_t phys_addr = pmm_alloc_frames ( frames_needed );

	/* Physical frames not allocated correctly */
	if ( IS_KERR ( phys_addr ) )
		return GET_KERR ( phys_addr );

	/* Physical -> Virtual Address */
	vaddr_t virt_heap_base = vmm_phys_to_virt ( phys_addr );

	/* Return any error */
	if ( IS_KERR_ ( virt_heap_base ) )
		return GET_KERR ( virt_heap_base );

	/* Fills out the kernel global heap state */
	heap->base = virt_heap_base;    /* Base address */
	heap->current = heap->base;     /* Current base */
	heap->limit = HEAP_INITIAL_SIZE;        /* Current limit size */
	heap->ready = 1;        /* Everything was already set */

	/* Defines the whole heap initially as a single chunk to be splitted */
	struct chunk_meta* meta = ( struct chunk_meta * ) heap->base;

	/* Sets it */
	meta->size = heap->limit;       /* The whole size */
	meta->is_free = 1;      /* It's free to use */
	meta->next = NULL;      /* There's no the next linked list chunk */

	/* Corrent initialized */
	return KERR_OK;
}
