#include <alloc/dispatch.h>
#include <alloc/internal.h>

static const char* heap_not_ready = "The give heap is not ready\n";

/**
 * Private heap creator
 * Private dispatcher
 *
 * Return -> `struct heap *`: Created heap context
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `alloc_heap_create`
 **/
struct heap*
_alloc_dispatch_heap_create ( void )
{
	return _heap_create (  );
}

/**
 * Private heap destroyer
 * Private dispatcher
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `alloc_heap_destroy`
 **/
kerr_t
_alloc_dispatch_heap_destroy ( struct heap* heap )
{

	KERR_RET_IF ( !heap->ready, KERR_NOTREADY, heap_not_ready );

	/* Get the function pointer from the internal operations */
	return heap->ops->destroy ( heap );
}

/**
 * Private heap memory allocator
 * Private dispatcher
 *
 * Return -> `void *`: Payload start pointer
 * Arguments:
 *   1. `struct heap* heap`: Heap context
 *   2. `size_t size`: Allocation size
 * Callers:
 *   - `alloc_heap_malloc`
 **/
void*
_alloc_dispatch_heap_malloc ( struct heap* heap, size_t size )
{

	KERR_RET_ENCODE_PTR_IF ( !heap->ready, KERR_NOTREADY, heap_not_ready );

	return heap->ops->malloc ( heap, size );
}

/**
 * Private heap memory liberator
 * Private dispatcher
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct heap* heap`: Heap context
 *   2. `void *`: Payload start
 * Callers:
 *   - `alloc_heap_mfree`
 **/
kerr_t
_alloc_dispatch_heap_mfree ( struct heap* heap, void* ptr )
{

	KERR_RET_IF ( !heap->ready, KERR_NOTREADY, heap_not_ready );

	return heap->ops->mfree ( ptr );
}

/**
 * Private heap grower
 * Private dispatcher
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct heap* heap`: Heap context
 *   2. `size_t frames_needed`: Amount of frames to be allocated
 * Callers:
 *   - `alloc_heap_grow`
 **/
kerr_t
_alloc_dispatch_heap_grow ( struct heap* heap, size_t frames_needed )
{

	KERR_RET_IF ( !heap->ready, KERR_NOTREADY, heap_not_ready );

	return heap->ops->grow ( heap, frames_needed );
}

/**
 * Private kernel memory allocator
 * Private dispatcher
 *
 * Return -> `void *`: Payload start pointer
 * Arguments:
 *   1. `size_t size`: Allocation size
 * Callers:
 *   - `k_malloc`
 **/
void*
_alloc_dispatch_k_malloc ( size_t size )
{

	/* Kernel global heap state */
	struct heap* kernel_heap = g_alloc->kernel.heap;

	/**
	 * Calls the dispatcher layer
	 *
	 * Organize the logic here for other allocators
	 **/
	return _alloc_dispatch_heap_malloc ( kernel_heap, size );
}

/**
 * Private kernel memory liberator
 * Private dispatcher
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void *`: Payload start
 * Callers:
 *   - `k_mfree`
 **/
kerr_t
_alloc_dispatch_k_mfree ( void* ptr )
{

	struct heap* kernel_heap = g_alloc->kernel.heap;

	/**
	 * Return the dispatcher
	 *
	 * Organize the logic here for other allocators
	 **/
	return _alloc_dispatch_heap_mfree ( kernel_heap, ptr );
}
