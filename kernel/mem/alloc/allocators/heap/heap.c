#include "heap.h"
#include <alloc/backend.h>
#include <alloc/allocators/heap.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/mem/vmm/map.h>
#include <rks/lib/string.h>

/* Operations */
const struct heap_ops ops = {
	.create = _heap_create,
	.destroy = _heap_destroy,
	.malloc = _heap_malloc,
	.free = _heap_free,
};

/* Prototypes */
static kerr_t _heap_destroy ( struct heap* heap );
static void* _heap_malloc ( struct heap* heap, size_t size );
static kerr_t _heap_free ( void* ptr );
static vaddr_t _heap_grow ( struct heap* heap, size_t frames_needed );

static __heap_alloc_and_map ( size_t frames_needed );
static void _heap_merge_chunks ( struct chunk_header* left,
                                 struct chunk_header* right );

/**
 * Heap backend context creator
 * Backend function
 *
 * Return -> `struct heap *`: Heap context pointer (opaque type only accessible through ALLOC subsystem)
 * Arguments:
 *   1. `void`
 * Callers:
 *   - ALLOC implementations
 **/
struct heap*
_heap_create ( void )
{

	/* Gets a mapped init heap region */
	vaddr_t virt_heap_base =
	        __heap_alloc_and_map ( FRAMES_BY_SIZE ( HEAP_INITIAL_SIZE ) );

	/* Return any error */
	KERR_PROPAGATE_ENCODE ( virt_heap_base );

	/* This heap struct starts at the start of this allocation */
	struct heap* this_heap = virt_heap_base;

	/* Arena region start */
	this_heap->arena_start =
	        ( char * ) virt_heap_base + sizeof ( struct heap );

	/* Current base */
	this_heap->current_arena = this_heap->arena_start;

	this_heap->limit = HEAP_INITIAL_SIZE;   /* Current limit size */
	this_heap->ops = &ops;  /* Global operations */
	this_heap->ready = 1;   /* Everything was already set */

	/* Defines the whole heap initially as a single chunk to be splitted */
	struct chunk_header* this_header =
	        ( struct chunk_header * ) this_heap->arena_start;

	/* Sets it */
	this_header->size = heap->limit;        /* The whole size */
	this_header->is_free = 1;       /* It's free to use */
	this_header->prev_free = 0;     /* There's no previous chunk */
	this_header->next_free = 0;     /* There's no next chunk, This is the first one */
	this_header->next = NULL;       /* The next chunk wasn't set yet */

	/* Return the reference */
	return heap;
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
	        ( struct chunk_header * ) heap->current_arena;

	/* Loop 'til manual break */
	for ( ;; )
	{

		/* Verifies this chunk availability */
		if ( current_chunk->is_free &&
		     PAYLOAD_SIZE ( current_chunk->size ) >= size )
		{

			/* Split this chunk if it's too big for the size */
			if ( current_chunk->size - CHUNK_SIZE ( size ) >=
			     CHUNK_LIMIT_BOUNDARY )
			{

				/**
                                 * Now we know the current chunk is far bigger than needed,
                                 * This is its updated size, where only the `size` is enough
                                 *
                                 * The macro will consider metadata and align it up
                                 **/
				size_t new_chunk_size = CHUNK_SIZE ( size );

				/* New chunk header metadata */
				struct chunk_header* new_chunk =
				        ( struct heap_meta * ) ( ( char * )
				                                 current_chunk +
				                                 new_chunk_size );

				/**
                                 * Size is simply the displacement ->
                                 * Of the updated current chunk's size
                                 **/
				new_chunk->size =
				        current_chunk->size - new_chunk_size;

				/* Just set. It's free */
				new_chunk->is_free = 1;
				new_chunk->prev_free = 0;       /* Current chunk */
				new_chunk->next_free = current_chunk->next_free;

				/**
                                 * As we splitted it, the next chunk is gonna be the current one
                                 * And this pass to be the next right there
                                 **/
				new_chunk->next = current_chunk->next;

				/* As it's free, sets its footer */
				this_footer_from_header ( new_chunk )->size =
				        new_chunk->size;

				/* It was shrank */
				current_chunk->size = new_chunk_size;
				current_chunk->next_free = 1;   /* New chunk */
				current_chunk->next = new_chunk;        /* The new chunk */
			}

			/* Now it's in use */
			current_chunk->is_free = 0;

			/* Return the base pointer */
			return this_header_from_payload ( current_chunk );
		}

		/* Tries growing the heap size if there's no a next chunk */
		if ( current_chunk->next == NULL )
		{

			/* At least one frame */
			kerr_t err =
			        _heap_grow ( heap, FRAMES_BY_SIZE ( size ) );

			/* Some error occurred */
			if ( IS_RET_OK ( err ) )
				return ENCODE_KERR ( err );

			/* Next chunk's base address */
			current_chunk->next = heap->current_arena;

			/* Next chunk's metadata */
			struct chunk_header* new_chunk =
			        ( struct chunk_header * ) current_chunk->next;

			/* The whole size to permit it to be splitted later */
			new_chunk->size =
			        SIZE_BY_FRAMES ( FRAMES_BY_SIZE ( size ) );
			new_chunk->is_free = 1; /* It's free for the next iteration */
			new_chunk->prev_free = current_chunk->is_free;
			new_chunk->next_free = 0;       /* The newest */
			new_chunk->next = NULL; /* The last element */
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
 * Heap backend memory freer
 * Backend function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void* ptr`: Payload base address
 * Callers:
 *   - ALLOC implementations
 **/
kerr_t
_heap_mfree ( void* ptr )
{

	/* Gets its header metadata */
	struct chunk_header* this_header = this_header_from_payload ( ptr );

	/* Set it as free to use */
	this_header->is_free = 1;

	/* Gets this chunk's footer */
	struct chunk_footer* this_footer =
	        this_footer_from_header ( this_header );

	/* Sets its size in the footer */
	this_footer->size = this_header->size;

	/* Previous chunk's footer (tags boundary) */
	struct chunk_footer* prev_footer =
	        prev_footer_from_header ( this_header );

	/* Previous chunk's header (from its footer) */
	struct chunk_header* prev_header =
	        this_header_from_footer ( prev_footer );

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

	/* Everything's okay */
	return KERR_OK;
}

/**
 * Heap area grower
 * Backend function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct heap* heap`: Heap to be grown
 *   2. `size_t frames_needed`: As it grows by frames, the minimum amount to be grown
 * Callers:
 *   - ALLOC implementations
 **/
kerr_t
_heap_grow ( struct heap* heap, size_t frames_needed )
{

	/* Gets another mapped region */
	vaddr_t virt_heap_current_base = __heap_alloc_and_map ( frames_needed );

	/* Locks it */
	heap->ready = 0;

	/* Updated current frame */
	heap->current_arena = virt_heap_base;

	/**
         * New limit
         *
         * - `FRAMES_BY_SIZE`: At least one frames
         * - `SIZE_BY_FRAMES`: Convert this amount in an absolute size
         **/
	heap->limit += SIZE_BY_FRAMES ( FRAMES_BY_SIZE ( size ) );

	/* Unlocks it to proceed */
	heap->ready = 1;

	/* Success */
	return KERR_OK;
}

/**
 * Heap helper used to get a mapped region memory
 * Private backend helper
 *
 * Return -> `vaddr_t`: New allocation base address
 * Arguments:
 *   1. `size_t frames_needed`: As it grows by frames, the minimum amount to be grown
 * Callers:
 *   - Private heap implementations
 **/
vaddr_t
__heap_alloc_and_map ( size_t frames_needed )
{

	/* Tries to allocate the requested amount of frames */
	paddr_t phys_addr = pmm_alloc_region ( frames_needed );

	/* Physical frames not allocated correctly */
	KERR_PROPAGATE_ENCODE ( phys_addr );

	/* Physical -> Virtual Address */
	vaddr_t virt_addr = vmm_phys_to_virt ( phys_addr );

	/* Return any error */
	KERR_PROPAGATE_ENCODE ( virt_addr );

	/* Return the mapped virtual address */
	return virt_addr;
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
	left->next_free = right->next_free;
	left->next = right->next;

	/* Clears the next header */
	memset ( right, 0, sizeof ( struct chunk_header ) );

	/* Just like its footer */
	memset ( this_footer_from_header ( right ), 0,
	         sizeof ( struct chunk_footer ) );
}
