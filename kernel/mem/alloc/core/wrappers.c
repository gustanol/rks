#include <alloc/dispatch.h>
#include <rks/kernel/mem/alloc.h>

/**
 * Public heap context creator
 * Public wrapper
 *
 * Return -> `HEAP *`: Heap context opaque pointer (outside this subsystem)
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
HEAP*
alloc_heap_create ( void )
{
	return _alloc_dispatch_heap_create (  );
}

/**
 * Public heap context destroyer
 * Public wrapper
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `HEAP* heap`: Heap context pointer type
 * Callers:
 *   - Many
 **/
kerr_t
alloc_heap_destroy ( HEAP* heap )
{

	/**
         * Validate arguments
         * This is going to repeat for next functions
         **/
	KERR_RET_IF ( heap == NULL, KERR_INVAL );
	KERR_RET_IF ( IS_KERR ( heap ), KERR_INVAL );

	return _alloc_dispatch_heap_destroy ( heap );
}

/**
 * Public heap memory allocator
 * Public wrapper
 *
 * Return -> `void *`: Allocated payload starter pointer
 * Arguments:
 *   1. `HEAP* heap`: Heap context pointer type
 *   2. `size_t size`: Allocation size
 * Callers:
 *   - Many
 **/
void*
alloc_heap_malloc ( HEAP* heap, size_t size )
{
	KERR_RET_ENCODE_IF ( heap == NULL, KERR_INVAL );
	KERR_RET_ENCODE_IF ( IS_KERR ( heap ), KERR_INVAL );
	KERR_RET_ENCODE_IF ( size == 0, KERR_INVAL );

	return _alloc_dispatch_heap_malloc ( heap, size );
}

/**
 * Public heap memory liberator
 * Public wrapper
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `HEAP* heap`: Heap context pointer type
 *   2. `void* ptr`: Payload starter pointer
 * Callers:
 *   - Many
 **/
kerr_t
alloc_heap_mfree ( HEAP* heap, void* ptr )
{
	KERR_RET_IF ( heap == NULL, KERR_INVAL );
	KERR_RET_IF ( IS_KERR ( heap ), KERR_INVAL );
	KERR_RET_IF ( ptr == NULL, KERR_INVAL );
	KERR_RET_IF ( IS_KERR ( ptr ), KERR_INVAL );

	return _alloc_dispatch_heap_mfree ( heap, ptr );
}

/**
 * Public heap grower
 * Public wrapper
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `HEAP* heap`: Heap context pointer type
 *   2. `size_t frames_needed`: Amount of frames to grow
 * Callers:
 *   - Many
 **/
kerr_t
alloc_heap_grow ( HEAP* heap, size_t frames_needed )
{
	KERR_RET_IF ( heap == NULL, KERR_INVAL );
	KERR_RET_IF ( IS_KERR ( heap ), KERR_INVAL );
	KERR_RET_IF ( frames_needed == 0, KERR_INVAL );

	return _alloc_dispatch_heap_grow ( heap, frames_needed );
}

/**
 * Public kernel memory allocator
 * Public wrapper
 *
 * Return -> `void *`: Allocated payload starter pointer
 * Arguments:
 *   1. `size_t size`: Allocation size
 * Callers:
 *   - Many
 **/
void*
k_malloc ( size_t size )
{
	KERR_RET_IF ( size == 0, KERR_INVAL );

	return _alloc_k_malloc ( size );
}
