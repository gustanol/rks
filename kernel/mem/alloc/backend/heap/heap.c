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

/* Prototypes */
static vaddr_t __heap_grow ( size_t frames_needed );

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

	/* Allocate the initial heap region */
	vaddr_t virt_heap_base =
	        __heap_grow ( FRAMES_BY_SIZE ( HEAP_INITIAL_SIZE ) );

	/* Return any error */
	if ( IS_KERR_ ( virt_heap_base ) )
		return GET_KERR ( virt_heap_base );

	/* Fills out the kernel global heap state */
	heap->base = virt_heap_base;    /* Base address */
	heap->current = heap->base;     /* Current base */
	heap->limit = HEAP_INITIAL_SIZE;        /* Current limit size */
	heap->ready = 1;        /* Everything was already set */

	/* Defines the whole heap initially as a single chunk to be splitted */
	struct chunk_header* meta = ( struct chunk_header * ) heap->base;

	/* Sets it */
	meta->size = heap->limit;       /* The whole size */
	meta->is_free = 1;      /* It's free to use */
	meta->prev_free = 0;    /* There's no previous chunk */
	meta->next = NULL;      /* The next chunk wasn't set yet */

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
	struct chunk_header* current_chunk =
	        ( struct chunk_header * ) heap->base;

	/* Loop 'til manual break */
	for ( ;; )
	{

		/* Verifies this chunk availability */
		if ( current_chunk->is_free &&
		     PAYLOAD_SIZE ( current_chunk->size ) >= size )
		{

			/* Split this chunk if it's too big for the size */
			if ( PAYLOAD_SIZE ( current_chunk->size ) - size >=
			     CHUNK_LIMIT_BOUNDARY )
			{


				/**
                                 * Now we know the current chunk is far bigger than needed,
                                 * This is its updated size, where only the `size` is enough
                                 *
                                 * The macro will consider metadata and align it up
                                 **/
				size_t new_current_chunk_size =
				        CHUNK_SIZE ( size );

				/* New chunk header metadata */
				struct chunk_header* new_chunk =
				        ( struct heap_meta * ) ( ( char * )
				                                 current_chunk +
				                                 new_current_chunk_size );

				/* Newer chunk metadata */

				/**
                                 * Size is simply the displacement
                                 * Of the updated current chunk's size
                                 **/
				new_chunk->size =
				        current_chunk->size -
				        new_current_chunk_size;

				/* Just set. It's free  */
				new_chunk->is_free = 1;

				/**
                                 * As we splitted, the next chunk is gonna be the current's one
                                 * And this pass to be the next right there
                                 **/
				new_chunk->next = current_chunk->next;

				/* It was shrank */
				current_chunk->size = new_current_chunk_size;
				current_chunk->next = new_chunk;        /* The new chunk */
			}

			/* Now it's in use */
			current_chunk->is_free = 0;

			/* Return the base pointer */
			return ( void * ) META_SHIFT ( current );
		}

		/* Tries growing the heap size if there's no a next chunk */
		if ( current_chunk->next == NULL )
		{
			/* At least one frame */
			vaddr_t virt_heap_base =
			        __heap_grow ( FRAMES_BY_SIZE ( size ) );

			/* Some error occurred */
			if ( IS_KERR ( virt_heap_base ) )
				return ENCODE_KERR ( GET_KERR
			                             ( virt_heap_base ) );

			heap->ready = 0;        /* Locks it */

			/* Updated current frame */
			heap->current = virt_heap_base;

			/**
                         * New limit
                         *
                         * - `FRAMES_BY_SIZE`: At least one frames
                         * - `SIZE_BY_FRAMES`: Convert this amount in an absolute size
                         **/
			heap->limit +=
			        SIZE_BY_FRAMES ( FRAMES_BY_SIZE ( size ) );

			heap->ready = 1;        /* Unlocks it again */

			/* Next chunk's base address */
			current_chunk->next = heap->current;

			/* Next chunk's metadata */
			struct chunk_header* meta =
			        ( struct chunk_header * ) current_chunk->next;

			/* The whole size to permit it to be splitted later */
			meta->size = SIZE_BY_FRAMES ( FRAMES_BY_SIZE ( size ) );
			meta->is_free = 1;      /* It's free for the next iteration */
			meta->next = NULL;      /* The last element */
		}

		/* Goes to the next chunk on the linked list */
		current_chunk = current_chunk->next;
	}

	/**
         * If not found or not allocated, probably out of memory
         *
         * This shound't be achievable considering errors during heap growns are treated before
         **/
	return ENCODE_KERR ( KERR_NOMEM );
}

/**
 * Heap helper used to grow the region
 * Private backend helper
 *
 * Return -> `vaddr_t`: New allocation base address
 * Arguments:
 *   1. `size_t frames_needed`: As it grows by frames, the minimum amount to be grown
 * Callers:
 *   - Internal heap backend functions
 **/
vaddr_t
__heap_grow ( size_t frames_needed )
{

	/* Tries to allocate the requested amount of frames */
	paddr_t phys_addr = pmm_alloc_frames ( frames_needed );

	/* Physical frames not allocated correctly */
	if ( IS_KERR ( phys_addr ) )

	        /* Note `phys_addr` is already an encoded error. Just make it explicity */
		return ENCODE_KERR ( GET_KERR ( phys_addr ) );

	/* Physical -> Virtual Address */
	vaddr_t virt_heap_base = vmm_phys_to_virt ( phys_addr );

	/* Return any error */
	if ( IS_KERR_ ( virt_heap_base ) )

	        /* Just like `phys_addr`, `virt_heap_base` is already an encoded error */
		return ENCODE_KERR ( GET_KERR ( virt_heap_base ) );

	/* After verifications, we can return it safely */
	return virt_heap_base;
}
