#include <kernel/mem/alloc.h>
#include <kernel/mem/pmm/frames.h>
#include <mem/vmm/paging/addr_space.h>

/**
 * Address space creator
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

kerr_t
_paging_destroy_space ( ADDR_SPACE* space )
{
	kerr_t err;

	err = pmm_free_frame ( ( paddr_t ) space->pml4_phys );
	if ( !IS_RET_OK ( err ) )
		return err;

	err = k_mfree ( space );
	return err;
}

kerr_t
_paging_switch_to_space ( ADDR_SPACE* space )
{
	paddr_t pml4 = ( paddr_t ) space->pml4;

	/* *INDENT-OFF* */

	__asm__ volatile (
		"mov %0, %%cr3"
		: "=r" ( pml4 )
	);

	/* *INDENT-ON* */
}
