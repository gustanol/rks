#include <pmm/dispatch.h>
#include <rks/kernel/mem/pmm.h>

/*
 * Public frame allocator
 * Public wrapper
 *
 * Return -> `paddr_t`: Frame physical base address
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 */
paddr_t
pmm_alloc_frame ( void )
{
	/* Allocates the next available frame */
	return _pmm_dispatch_alloc_frame (  );
}

/*
 * Public frame liberator
 * Public wrapper
 *
 * Return -> `u32`: Return code
 *   - 1 on success
 *   - 0 on fail
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame physical base address
 * Callers:
 *   - Many
 */
u32
pmm_free_frame ( paddr_t phys_addr )
{
	/* Frees the passed frame (by its physical base address) */
	return _pmm_dispatch_free_frame ( phys_addr );
}

/*
 * Public region map freer
 * Public wrapper
 *
 * Return -> `size_t`: Liberated frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 */
size_t
pmm_free_region ( paddr_t phys_addr, size_t size )
{
	/* Frees the whole region */
	return _pmm_dispatch_region_free ( phys_addr, size );
}

/*
 * Public region map setter
 * Public wrapper
 *
 * Return -> `size_t`: Set frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 */
size_t
pmm_fill_region ( paddr_t phys_addr, size_t size )
{
	/* Frees the whole region */
	return _pmm_dispatch_region_used ( phys_addr, size );
}

/*
 * Public helper to return the total memory size
 * Public wrapper
 *
 * Return -> `size_t`: Total memory
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 */
size_t
pmm_get_total_memory ( void )
{
	/* Gets the total memory size and returns it */
	return _pmm_dispatch_get_total_memory (  );
}
