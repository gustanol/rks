#include <pmm/dispatch.h>
#include <rks/kernel/mem/pmm/frames.h>
#include <rks/kernel/mem/pmm/memory.h>

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

paddr_t
pmm_alloc_frames ( size_t frames_needed )
{

	KERR_RET_ENCODE_IF ( frames_needed == NULL, KERR_INVAL, "" );
	KERR_RET_ENCODE_IF ( IS_KERR ( frames_needed ), KERR_INVAL, "" );

	return _pmm_dispatch_alloc_frames ( frames_needed );
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

	KERR_RET_IF ( phys_addr == NULL, KERR_INVAL,
	              "Physical address is NULL" );
	KERR_RET_IF ( IS_KERR ( heap ), KERR_INVAL,
	              "Physical address is KERR" );

	return _pmm_dispatch_free_frame ( phys_addr );
}

/**
 * Public region map setter
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 **/
kerr_t
pmm_fill_region ( paddr_t phys_addr, size_t size )
{

	KERR_RET_ENCODE_IF ( phys_addr == NULL, KERR_INVAL,
	                     "Physical address is NULL" );
	KERR_RET_ENCODE_IF ( IS_KERR ( phys_addr ), KERR_INVAL,
	                     "Physical address is KERR" );

	KERR_RET_ENCODE_IF ( size == NULL, KERR_INVAL,
	                     "Size of the region to be filled is NULL" );
	KERR_RET_ENCODE_IF ( IS_KERR ( size ), KERR_INVAL,
	                     "Size of the region to be filled is KERR" );

	return _pmm_dispatch_fill_region ( phys_addr, size );
}

/**
 * Public region map freer
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Region physical base address
 *   2. `size_t size`: Region size
 * Callers:
 *   - Many
 **/
kerr_t
pmm_free_region ( paddr_t phys_addr, size_t size )
{

	KERR_RET_ENCODE_IF ( phys_addr == NULL, KERR_INVAL,
	                     "Physical address is NULL" );
	KERR_RET_ENCODE_IF ( IS_KERR ( phys_addr ), KERR_INVAL,
	                     "Physical address is KERR" );

	KERR_RET_ENCODE_IF ( size == NULL, KERR_INVAL,
	                     "Size of the region to be freed is NULL" );
	KERR_RET_ENCODE_IF ( IS_KERR ( size ), KERR_INVAL,
	                     "Size of the region to be freed is KERR" );

	return _pmm_dispatch_free_region ( phys_addr, size );
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
	return _pmm_dispatch_get_total_memory (  );
}

/**
 * Public helper to return the available memory size
 * Public wrapper
 *
 * Return -> `size_t`: Available memory
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
size_t
pmm_get_available_memory ( void )
{
	return _pmm_dispatch_get_available_memory (  );
}
