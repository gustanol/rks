#include <vmm/dispatch.h>
#include <vmm/internal.h>

/**
 * Private address space creator
 * Private dispatcher
 *
 * Return `ADDR_SPACE *`: Address space pointer
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `vmm_create_space`
 **/
ADDR_SPACE*
_vmm_dispatch_create_space ( void )
{

	KERR_RET_ENCODE_PTR_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* Calls the operation function to create a new space */
	return g_vmm->ops->create_space (  );
}

/**
 * Private address space destroyer
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Space to be destroyed
 * Callers:
 *   - `vmm_destroy_space`
 **/
kerr_t
_vmm_dispatch_destroy_space ( ADDR_SPACE* space )
{

	KERR_RET_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* Destroys this space */
	return g_vmm->ops->destroy_space ( space );
}

/**
 * Private address space switcher
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to be switched to
 * Callers:
 *   - `vmm_switch_to_space`
 **/
kerr_t
_vmm_dispatch_switch_to_space ( ADDR_SPACE* space )
{

	KERR_RET_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* Switches the caller core to this space */
	return g_vmm->ops->switch_to_space ( space );
}

/**
 * Private virtual memory mapper
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `uint32_t flags`: Extra flags
 * Callers:
 *   - `vmm_map`
 **/
kerr_t
_vmm_dispatch_map ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, uint32_t flags )
{

	KERR_RET_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* Gets the mapped HHDM offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Calls the operation function */
	return g_vmm->ops->map ( space, virt_addr, phys_addr, hhdm_offset, flags );
}

/**
 * Private virtual memory unmapper
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to unmap the address from
 *   2. `vaddr_t virt_addr`: Virtual address
 * Callers:
 *   - `vmm_unmap`
 **/
kerr_t
_vmm_dispatch_unmap ( ADDR_SPACE* space, vaddr_t virt_addr )
{

	KERR_RET_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* HHDM_offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	return g_vmm->ops->unmap ( space, virt_addr, hhdm_offset );
}

/**
 * Private virtual memory remapper
 * Private dispatcher
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `uint32_t flags`: Extra flags
 * Callers:
 *   - `vmm_map`
 **/
kerr_t
_vmm_dispatch_remap ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, uint32_t flags )
{

	KERR_RET_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* HHDM offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	return ( int ) g_vmm->ops->remap ( space, virt_addr, phys_addr, hhdm_offset, flags );
}

/**
 * Private physical address conversion
 * Private dispatcher
 *
 * Return -> `paddr_t`: Converted physical address
 * Arguments:
 *   1. `ADDR_SPACE* space`: Virtual address' address space
 *   2. `vaddr_t virt_addr`: The virtual address
 * Callers:
 *   - `vmm_virt_to_phys`
 **/
paddr_t
_vmm_dispatch_virt_to_phys ( ADDR_SPACE* space, vaddr_t virt_addr )
{

	KERR_RET_ENCODE_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Converts the virtual address to its associated physical one */
	return g_vmm->ops->virt_to_phys ( space, virt_addr, hhdm_offset );
}

/**
 * Private virtual address conversion
 * Private dispatcher
 *
 * Return -> `vaddr_t`: Converted physical address
 * Arguments:
 *   1. `vaddr_t virt_addr`: The virtual address
 * Callers:
 *   - `vmm_virt_to_phys`
 **/
vaddr_t
_vmm_dispatch_phys_to_virt ( paddr_t phys_addr )
{

	KERR_RET_ENCODE_IF ( !g_vmm->ready, KERR_NOTREADY, "VMM not ready" );

	/* Gets the HHDM offset to serve as the conversion base */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Converts it */
	return g_vmm->ops->phys_to_virt ( phys_addr, hhdm_offset );
}
