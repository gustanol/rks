#include <pmm/dispatch.h>
#include <pmm/internal.h>

/**
 * Private physical frame allocator
 * Private dispatcher
 *
 * Return -> `paddr_t`: Allocated physical frame
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `pmm_alloc_frame`
 **/
paddr_t
_pmm_dispatch_alloc_frame ( void )
{
	u32 total_frames = g_pmm->frames.total;

	/* Allocates a new physical frame */
	paddr_t phys_addr = g_pmm->ops->alloc_frame ( total_frames );

	/* If correctly allocated, there's no kerr_t */
	if ( !IS_KERR ( phys_addr ) )

	        /* Update the amount of used frames */
		g_pmm->frames.used++;

	/* Returns the actual value */
	return phys_addr;
}

/**
 * Private physical frame freer
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame physical address
 * Callers:
 *   - `pmm_free_frame`
 **/
kerr_t
_pmm_dispatch_free_frame ( paddr_t phys_addr )
{
	u32 total_frames = g_pmm->frames.total;

	/* Frees the requested frame */
	kerr_t ret = g_pmm->ops->free_frame ( phys_addr, total_frames );

	/* If correct freed */
	if ( ret == KERR_OK )

	        /* Updates the amount of frames used */
		g_pmm->frames.used--;

	/* Returns it */
	return ret;
}

/**
 * Private region map freer
 * Private dispatcher
 *
 * Return -> `u32`: Successfully cleared frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_free`
 **/
u32
_pmm_dispatch_free_region ( paddr_t phys_addr, size_t size )
{
	u32 total_frames = g_pmm->frames.total;

	/* Tries to free this region */
	u32 cleared_frames =
	        g_pmm->ops->free_region ( phys_addr, size, total_frames );

	/* No returning error */
	if ( !IS_KERR ( cleared_frames ) )

	        /* Decreases the amount of used frames */
		g_pmm->frames.used -= size;

	return cleared_frames;
}

/**
 * Private region map setter
 * Private dispatcher
 *
 * Return -> `u32`: Successfully set frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_used`
 **/
kerr_t
_pmm_dispatch_fill_region ( paddr_t phys_addr, size_t** size )
{
	u32 total_frames = g_pmm->frames.total;

	/* Tries to fill this region */
	u32 set_frames =
	        g_pmm->ops->fill_region ( phys_addr, size, total_frames );

	/* No returning error */
	if ( !IS_KERR ( set_frames ) )

	        /* Increases the amount of used frames */
		g_pmm->frames.used += size;

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
