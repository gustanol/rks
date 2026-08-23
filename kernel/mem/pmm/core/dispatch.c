#include <kernel/mem/pmm/frames.h>
#include <pmm/dispatch.h>
#include <pmm/internal.h>

/* Inline function to get the `pmm_ops` pointer */
static inline struct pmm_ops*
get_ops ( void )
{
	return ( struct pmm_ops * ) g_pmm->ops;
}

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

	KERR_RET_ENCODE_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	uint32_t total_frames = g_pmm->frames.total;

	/* Allocates a new physical frame */
	paddr_t phys_addr = get_ops (  )->alloc_frame (  );

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

	KERR_RET_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	uint32_t total_frames = g_pmm->frames.total;

	/* Frees the requested frame */
	kerr_t ret = get_ops (  )->free_frame ( phys_addr, total_frames );

	/* If correct freed */
	if ( IS_RET_OK ( ret ) )

	    /* Updates the amount of frames used */
		g_pmm->frames.used--;

	/* Return it */
	return ret;
}

/**
 * Private physical contiguous region allocator
 * Private dispatcher
 *
 * Return -> `paddr_t`: Region start physical address
 * Arguments:
 *   1. `size_t frames_needed`: Region size in frames
 * Callers:
 *   - `pmm_alloc_region`
 **/
paddr_t
_pmm_dispatch_alloc_region ( size_t frames_needed )
{

	KERR_RET_ENCODE_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	uint32_t total_frames = g_pmm->frames.total;

	/* Region start */
	paddr_t phys_addr = get_ops (  )->alloc_region ( frames_needed, total_frames );

	/* Just update the global state on correct allocation */
	if ( !IS_KERR ( phys_addr ) )
		g_pmm->frames.used += frames_needed;

	return phys_addr;
}

/**
 * Private region map setter
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_used`
 **/
kerr_t
_pmm_dispatch_fill_region ( paddr_t phys_addr, size_t size )
{

	KERR_RET_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	uint32_t total_frames = g_pmm->frames.total;

	/* Tries to fill this region */
	kerr_t ret = get_ops (  )->fill_region ( phys_addr, size, total_frames );

	/* No returning error */
	if ( IS_RET_OK ( ret ) )

	    /* Increases the amount of used frames */
		g_pmm->frames.used += size;

	return ret;
}

/**
 * Private region map freer
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 * Callers:
 *   - `pmm_region_free`
 **/
kerr_t
_pmm_dispatch_free_region ( paddr_t phys_addr, size_t size )
{

	KERR_RET_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	uint32_t total_frames = g_pmm->frames.total;

	/* Tries to free this region */
	kerr_t ret = get_ops (  )->free_region ( phys_addr, size, total_frames );

	/* No returning error */
	if ( IS_RET_OK ( ret ) )

	    /* Decreases the amount of used frames */
		g_pmm->frames.used -= size;

	return ret;
}

/**
 * Private physical memory getter
 * Private dispatcher
 *
 * Return -> `size_t`: Total memory size
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `pmm_get_total_memory`
 **/
size_t
_pmm_dispatch_get_total_memory ( void )
{

	KERR_RET_ENCODE_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	/* Gets the total stored memory size */
	size_t total_memory = g_pmm->memory.total;

	/* Return it back */
	return total_memory;
}

/**
 * Private physical available memory getter
 * Private dispatcher
 *
 * Return -> `size_t`: Total memory size
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `pmm_get_avaiable_memory`
 **/
size_t
_pmm_dispatch_get_available_memory ( void )
{

	KERR_RET_ENCODE_IF ( !g_pmm->ready, KERR_NOTREADY, "PMM not ready" );

	size_t total_memory = g_pmm->memory.total;
	uint32_t frames_used = g_pmm->frames.used;

	/* Return it */
	return total_memory - SIZE_BY_FRAMES ( frames_used );
}
