#include <rks/kernel/mem/alloc.h>
#include <x86/mem/vmm/paging/internal/addr_space.h>

/**
 * x86 address space creator
 * Backend function
 *
 * Return -> `ADDR_SPACE *`: Address space pointer
 * Arguments:
 *   1. `void`
 * Callers:
 *   - VMM internals
 **/
ADDR_SPACE*
_paging_create_space ( void )
{

	/* Allocate the PML4 for this address space */
	paddr_t pml4_phys = pmm_alloc_frame (  );

	/* Stop on error */
	if ( IS_KERR ( pml4_phys ) )

	        /* Already encoded */
		return pml4_phys;

	/* Allocate a new address space */
	ADDR_SPACE* space = ( ADDR_SPACE * ) k_malloc ( sizeof ( ADDR_SPACE ) );

	/* Fills it out */
	space->pml4_phys = pml4_phys;

	/* Return the allocate space */
	return space;
}
