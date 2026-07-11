#include "heap.h"
#include <alloc/backend.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/mem/vmm/map.h>

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
	meta->size = CHUNK_SIZE ( heap->limit );        /* The whole size */
	meta->is_free = 1;      /* It's free to use */
	meta->next = NULL;      /* There's no the next linked list chunk */

	/* Corrent initialized */
	return KERR_OK;
}

/**
 * Heap backend memory allocator
 * Backend function
 *
 * Return -> `void *`: Pointer to allocated object
 * Arguments:
 *   1. `struct heap* heap`: Heap state
 *   2. `size_t size`: Object size
 * Callers:
 *   - ALLOC implementations
 **/
void*
_heap_malloc ( struct heap* heap, size_t size )
{

	/* The heap state is not prepared */
	if ( !heap->ready )
		return ENCODE_KERR ( KERR_INVAL );

	/* Current chunk's metadata */
	struct chunk_meta* current_chunk = heap->base;

	/* Loop 'til manual break */
	for ( ;; )
	{

		/* Verifies this chunk availability */
		if ( current_chunk->is_free && current_chunk->size >= size )
		{

			/* Split this chunk if it's too big for the size */
			if ( current_chunk->size >=
			     CHUNK_SIZE ( size + CHUNK_LIMIT_BOUNDARY ) )
			{

				/* New chunk metadata */
				struct chunk_meta* new_chunk =
				        ( struct heap_meta * ) ( ( char * )
				                                 current_chunk +
				                                 CHUNK_SIZE
				                                 ( size ) );

				/* Newer chunk metadata */
				/* Size is simply the displacement */
				new_chunk->size =
				        CHUNK_SIZE ( CHUNK_LIMIT_BOUNDARY );

				/* Just set. It's free  */
				new_chunk->is_free = 1;

				/**
                                 * As we splitted, the next chunk is gonna be the current's one
                                 * And this pass to be the next right there
                                 **/
				new_chunk->next = current_chunk->next;

				/* Updates on the current chunk metadata */
				current_chunk->size = size;     /* It was shrank */
				current_chunk->next = new_chunk;        /* The new chunk */
			}

			current_chunk->is_free = 0;     /* Now it's in use */

			/* Return the base pointer */
			return ( void * ) META_SHIFT ( current );
		}

		/* Goes to the next chunk on the linked list */
		current_chunk = current_chunk->next;

		/* Tries growing the heap size if there's no a next chunk */
		/* NOT IMPLEMENTED YET */
		if ( current_chunk == NULL )
			__heap_grow (  );
	}

	/**
         * If not found or not allocated, probably out of memory
         *
         * This shound't be achievable considering errors during heap growns are treated before
         **/
	return ENCODE_KERR ( KERR_NOMEM );
}
