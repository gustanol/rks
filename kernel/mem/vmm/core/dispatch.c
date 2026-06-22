#include <vmm/dispatch.h>

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
	/* Calls the operation function to create a new space */
	return ( ADDR_SPACE * ) g_vmm->ops->create_space (  );
}

/**
 * Private address space destroyer
 * Private dispatcher
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* space`: Space to be destroyed
 * Callers:
 *   - `vmm_destroy_space`
 **/
void
_vmm_dispatch_destroy_space ( ADDR_SPACE* space )
{
	/* Destroys this space */
	g_vmm->ops->destroy_space ( space );
}

/**
 * Private address space switcher
 * Private dispatcher
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to be switched to
 * Callers:
 *   - `vmm_switch_to_space`
 **/
void
_vmm_dispatch_switch_to_space ( ADDR_SPACE* space )
{
	/* Switches the caller core to this space */
	g_vmm->ops->switch_to_space ( space );
}

/**
 * Private virtual memory mapper
 * Private dispatcher
 *
 * Return -> `int`:
 *   - 1: Successfully mapped
 *   - 0: Not mapped
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `u32 flags`: Extra flags
 * Callers:
 *   - `vmm_map`
 **/
int
_vmm_dispatch_map ( ADDR_SPACE* space, vaddr_t virt_addr,
                    paddr_t phys_addr, u32 flags )
{
	/* Gets the mapped HHDM offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Calls the operation function */
	return ( int ) g_vmm->ops->map ( space, virt_addr, phys_addr,
	                                 hhdm_offset, flags );
}

/**
 * Private virtual memory unmapper
 * Private dispatcher
 *
 * Return -> `int`:
 *   - 1: Successfully unmapped
 *   - 0: Not unmapped
 *     - Probably, any upper layer were not mapped yet
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to unmap the address from
 *   2. `vaddr_t virt_addr`: Virtual address
 * Callers:
 *   - `vmm_unmap`
 **/
int
_vmm_dispatch_unmap ( ADDR_SPACE* space, vaddr_t virt_addr )
{
	/* HHDM_offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	return ( int ) g_vmm->ops->unmap ( space, virt_addr, hhdm_offset );
}

/**
 * Private virtual memory remapper
 * Private dispatcher
 *
 * Return -> `int`:
 *   - 1: Successfully remapped
 *   - 0: Not remapped
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `u32 flags`: Extra flags
 * Callers:
 *   - `vmm_map`
 **/
int
_vmm_dispatch_remap ( ADDR_SPACE* space, vaddr_t virt_addr,
                      paddr_t phys_addr, u32 flags )
{
	/* HHDM offset */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	return ( int ) g_vmm->ops->remap ( space, virt_addr, phys_addr,
	                                   hhdm_offset, flags );
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

	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Converts the virtual address to its associated physical one */
	return ( paddr_t ) g_vmm->ops->virt_to_phys ( space, virt_addr,
	                                              hhdm_offset );
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
	/* Gets the HHDM offset to serve as the conversion base */
	vaddr_t hhdm_offset = g_vmm->hhdm_offset;

	/* Converts it */
	return ( vaddr_t ) g_vmm->ops->phys_to_virt ( phys_addr, hhdm_offset );
}
