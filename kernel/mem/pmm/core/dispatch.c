#include <pmm/dispatch.h>
#include <pmm/internal.h>

/**
 * Private physical frame allocator
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t ** page`: Page double pointer to be modified
 * Callers:
 *   - `pmm_alloc_frame`
 **/
kerr_t
_pmm_dispatch_alloc_frame ( paddr_t** page )
{
	/* Allocates a new physical frame */
	kerr_t ret = g_pmm->ops->alloc_frame ( page );

	/* If correctly allocated */
	if ( ret == KERR_OK )

	        /* Updates the amount of used frames */
		g_pmm->frames.used++;

	/* Returns the actual value */
	return err;
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
	/* Frees the requested frame */
	kerr_t ret = g_pmm->ops->free_frame ( phys_addr );

	/* Not freed */
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
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t ** size`: Region size to be modified to the size freed
 * Callers:
 *   - `pmm_region_free`
 **/
kerr_t
_pmm_dispatch_free_region ( paddr_t phys_addr, size_t** size )
{
	/* Tries to free this region */
	kerr_t ret = g_pmm->ops->free_region ( phys_addr, size );

	/* Decreases the amount of used frames */
	g_pmm->frames.used -= **size;

	/* Returning code */
	return ret;
}

/**
 * Private region map setter
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t ** size`: Region size to be modified to the size filled
 * Callers:
 *   - `pmm_region_used`
 **/
kerr_t
_pmm_dispatch_fill_region ( paddr_t phys_addr, size_t** size )
{
	/* Tries to fill this region */
	kerr_t ret = g_pmm->ops->fill_region ( phys_addr, size );

	/* Increases the amount of used frames */
	g_pmm->frames.used += **size;

	/* Returns it */
	return ret;
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
