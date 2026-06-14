#include <pmm/dispatch.h>

/**
 * Private physical frame allocator
 * Private dispatcher
 *
 * Return -> `paddr_t`: Physical frame base address
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `pmm_alloc_frame`
 **/
paddr_t
_pmm_dispatch_alloc_frame ( void )
{
	/* Allocates a new physical frame */
	paddr_t frame_phys_addr = g_pmm->ops->alloc_frame (  );

	/* Not allocated */
	if ( !frame_phys_addr )
		return 0;

	/* Update the amount of frames used */
	g_pmm->frames.used++;

	/* Cast and return it */
	return frame_phys_addr;
}

/**
 * Private physical frame freer
 * Private dispatcher
 *
 * Return -> `int`: Return code
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame physical address
 * Callers:
 *   - `pmm_free_frame`
 **/
u32
_pmm_dispatch_free_frame ( paddr_t phys_addr )
{
	/* Frees the requested frame */
	u32 res = g_pmm->ops->free_frame ( phys_addr );

	/* Not freed */
	if ( !res )
		return 0;

	/* Update the amount of frames used */
	g_pmm->frames.used--;

	/* Success response */
	return 1;
}

/**
 * Private region map freer
 * Private dispatcher
 *
 * Return -> `size_t`: Cleared frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_free`
 **/
size_t
_pmm_dispatch_free_region ( paddr_t phys_addr, size_t size )
{
	/* Gets the amount of frames that were correctly freed */
	size_t cleared_frames = g_pmm->ops->free_region ( phys_addr, size );

	/* Decrease the amount of used frames */
	g_pmm->frames.used -= cleared_frames;

	/* Return it */
	return cleared_frames;
}

/**
 * Private region map setter
 * Private dispatcher
 *
 * Return -> `size_t`: Set frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_used`
 **/
size_t
_pmm_dispatch_fill_region ( paddr_t phys_addr, size_t size )
{
	/* Gets the amount of frames that were correctly set */
	size_t set_frames = g_pmm->ops->fill_region ( phys_addr, size );

	/* Increases the amount of used frames */
	g_pmm->frames.used += set_frames;

	/* Return it */
	return set_frames;
}

/**
 * Public physical memory getter
 * Public wrapper
 *
 * Return -> `size_t`: The total memory size
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `pmm_get_total_memory`
 **/
size_t
_pmm_dispatch_get_total_memory ( void )
{
	/* Gets the total stored memory size */
	size_t total_memory = g_pmm->memory.total;

	/* Return it back */
	return total_memory;
}
