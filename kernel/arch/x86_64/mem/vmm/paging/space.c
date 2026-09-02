#include "addr_space.h"
#include <kernel/mem/alloc.h>
#include <kernel/mem/pmm/frames.h>

/**
 * Paging backend address space creator
 *
 * Implements the VMM backend space creator interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_CREATE_SPACE ( _paging_create_space )
{

	/* Allocates a new physical address for the PML4 */
	paddr_t pml4_phys = pmm_alloc_frame (  );

	/* Stop on error */
	if ( IS_KERR ( pml4_phys ) )
		return ENCODE_KERR_PTR ( GET_KERR ( pml4_phys ) );

	/* Allocates a new address space */
	struct addr_space* space = ( struct addr_space * ) k_malloc ( sizeof ( struct addr_space ) );

	/* Fills it out */
	space->pml4_phys = pml4_phys;

	/* Return the allocate space */
	return space;
}

/**
 * Paging backend address space destroyer
 *
 * Implements the VMM backend space destroyer interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_DESTROY_SPACE ( _paging_destroy_space )
{

	kerr_t err;

	/* Tries to free the PML4 physical address */
	err = pmm_free_frame ( ( paddr_t ) space->pml4_phys );
	KERR_RET_IF ( !IS_RET_OK ( err ), err,
	    "Couldn't free the given address space's PML4 physical address" );

	/* Frees the space itself */
	err = k_mfree ( space );
	return err;
}

/**
 * Paging backend address space switcher
 *
 * Implements the VMM backend space switcher interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_SWITCH_TO_SPACE ( _paging_switch_to_space )
{

	/* Get the PML4 physical address from the given address space */
	paddr_t pml4 = ( paddr_t ) space->pml4_phys;

	/* *INDENT-OFF* */

	/**
	 * Note the active PML4 pointer is store in CR3 register
	 * Simply overwrites it with the PML4 physical address goten previously
	 **/
	__asm__ volatile (
		"movq %0, %%cr3"
		: "=r" ( pml4 )
	);

	/* *INDENT-ON* */
}
