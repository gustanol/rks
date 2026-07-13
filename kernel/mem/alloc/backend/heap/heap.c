#include "heap.h"
#include <alloc/backend.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/mem/vmm/map.h>
#include <rks/lib/string.h>

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
static void __heap_merge_chunks ( struct chunk_header* left,
                                  struct chunk_header* right );

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

kerr_t
_heap_free ( void* ptr )
{
	/* If pointer is an actual address, return the invalid argument error */
	if ( ptr == NULL || IS_KERR ( ptr ) )
		return KERR_INVAL;

	/* Gets its header metadata */
	struct chunk_header* this_header =
	        ( struct chunk_header * ) HEADER_FROM_PAYLOAD ( ptr );

	/* Set it as free to use */
	this_header->is_free = 1;

	/* Gets this chunk's footer */
	struct chunk_footer* this_footer =
	        ( struct chunk_footer * ) GET_THIS_FOOTER ( this_header );

	/* Sets its size in the footer */
	this_footer->size = this_header->size;

	/* Previous chunk's footer (tags boundary) */
	struct chunk_footer* prev_footer =
	        PREV_FOOTER_FROM_HEADER ( this_header );

	/* Previous chunk's header (from its footer) */
	struct chunk_header* prev_header =
	        THIS_HEADER_FROM_FOOTER ( prev_footer );

	/* Next chunk's header */
	struct chunk_header* next_header = this_header->next;

	/**
         * NOTE: Now we must verify if the neighbors chunks are also free
         * This avoids keeping consecutive free chunks by merging them,
         * Allowing future allocations to split them once again
         **/

	/* Is the previous chunk free? */
	if ( this_header->prev_free )
	{
		/* Previous + Current */
		__heap_merge_chunks ( prev_header, this_header );

		/* Is the next free? */
		if ( header_meta->next_free )

		        /* Previous (already merged) + Next */
			__heap_merge_chunks ( prev_header, next_header );
	}

	/* Now, is the next free */
	else if ( this_header->next_free )

	        /* As the previous chunk isn't free, merge the Current + Next */
		__heap_merge_chunks ( this_header, next_header );

	/* If neither of them are free, we can update 'em safely */
	else
	{
		prev_header->next_free = 1;
		next_header->prev_free = 1;
	}
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

/**
 * Receiving two headers, merges them into one, always modifying the left one
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct chunk_header* left`: Left chunk header to be modified
 *   2. `struct chunk_header* right`: Right chunk header to be the new boundary
 * Callers:
 *   - `_heap_free`
 **/
static void
__heap_merge_chunks ( struct chunk_header* left, struct chunk_header* right )
{
	/* Increases the whole chunk size */
	left->size += right->size;

	/* "Next of the next" update */
	left->next_free += right->next_free;
	left->next = right->next;

	/* Clears the next header */
	memset ( right, 0, sizeof ( struct chunk_header ) );

	/* Just like its footer */
	memset ( THIS_FOOTER_FROM_HEADER ( right ), 0,
	         sizeof ( struct chunk_footer ) );
}
