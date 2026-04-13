#include <mem/early_alloc.h>

static u8 early_heap[EARLY_HEAP_SIZE];
static size_t early_ptr = 0;

/*
 * Early boot allocator
 * Used to allocate some structures before the PMM is enabled
 *
 * -> void *: Base pointer to the reserved memory
 * @size: Object size to be allocated
 */
void*
early_alloc (size_t size)
{
	/* Get the current base address of our "early heap" */
	void* ptr = &early_heap[early_ptr];

	/*
	 * Advances the pointer
	 *
	 * Note it only needs to go forward
	 */
	early_ptr += size;
	return ptr;
}
