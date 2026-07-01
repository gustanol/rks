#include <pmm/dispatch.h>
#include <rks/kernel/mem/pmm.h>

/**
 * Public frame allocator
 * Public wrapper
 *
 * Return -> `paddr_t`: Physical frame base address
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
paddr_t
pmm_alloc_frame ( void )
{
	return _pmm_dispatch_alloc_frame ( page );
}

/**
 * Public frame liberator
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame physical base address
 * Callers:
 *   - Many
 **/
kerr_t
pmm_free_frame ( paddr_t phys_addr )
{
	return _pmm_dispatch_free_frame ( phys_addr );
}

/**
 * Public region map freer
 * Public wrapper
 *
 * Return -> `u32`: Freed frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 **/
u32
pmm_free_region ( paddr_t phys_addr, size_t size )
{
	return _pmm_dispatch_free_region ( phys_addr, size );
}

/**
 * Public region map setter
 * Public wrapper
 *
 * Return -> `u32`: Filled frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 **/
u32
pmm_fill_region ( paddr_t phys_addr, size_t size )
{
	return _pmm_dispatch_fill_region ( phys_addr, size );
}

/**
 * Public helper to return the total memory size
 * Public wrapper
 *
 * Return -> `size_t`: Total memory
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
size_t
pmm_get_total_memory ( void )
{
	/* Gets the total memory size and returns it */
	return _pmm_dispatch_get_total_memory (  );
}
