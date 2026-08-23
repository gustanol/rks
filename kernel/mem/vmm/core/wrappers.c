#include <kernel/mem/vmm/addr_space.h>
#include <kernel/mem/vmm/map.h>
#include <vmm/dispatch.h>

/**
 * Public address space creator
 * Public wrapper
 *
 * Return -> `ADDR_SPACE *`: Address space pointer
 * Arguments:
 *   1. `void`
 * Callers:
 *   - Many
 **/
ADDR_SPACE*
vmm_create_space ( void )
{
	return _vmm_dispatch_create_space (  );
}

/**
 * Public address space destroyer
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Space to be destroyed
 * Callers:
 *   - Many
 **/
kerr_t
vmm_destroy_space ( ADDR_SPACE* space )
{
	return _vmm_dispatch_destroy_space ( space );
}

/**
 * Public address space switcher
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to be switched to
 * Callers:
 *   - Many
 **/
kerr_t
vmm_switch_to_space ( ADDR_SPACE* space )
{
	return _vmm_dispatch_switch_to_space ( space );
}

/**
 * Public virtual memory mapper
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `uint32_t flags`: Extra flags
 * Callers:
 *   - Many
 **/
kerr_t
vmm_map ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, uint32_t flags )
{
	return _vmm_dispatch_map ( space, virt_addr, phys_addr, flags );
}

/**
 * Public virtual memory unmapper
 * Public wrappers
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to unmap the address from
 *   2. `vaddr_t virt_addr`: Virtual address
 * Callers:
 *   - Many
 **/
kerr_t
vmm_unmap ( ADDR_SPACE* space, vaddr_t virt_addr )
{
	return _vmm_dispatch_unmap ( space, virt_addr );
}

/**
 * Public virtual memory remapper
 * Public wrapper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the address to
 *   2. `vaddr_t virt_addr`: Virtual address to map
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `uint32_t flags`: Extra flags
 * Callers:
 *   - Many
 *
 * - Should be used to overmap already-mapped addresses
 **/
kerr_t
vmm_remap ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, uint32_t flags )
{
	return _vmm_dispatch_remap ( space, virt_addr, phys_addr, flags );
}

/**
 * Public physical address conversion
 * Public wrapper
 *
 * Return -> `paddr_t`: Converted physical address
 * Arguments:
 *   1. `ADDR_SPACE* space`: Virtual address' address space
 *   2. `vaddr_t virt_addr`: The virtual address
 * Callers:
 *   - Many
 **/
paddr_t
vmm_virt_to_phys ( ADDR_SPACE* space, vaddr_t virt_addr )
{
	return _vmm_dispatch_virt_to_phys ( space, virt_addr );
}

/**
 * Public virtual address conversion
 * Public wrapper
 *
 * Return -> `vaddr_t`: Converted physical address
 * Arguments:
 *   1. `vaddr_t virt_addr`: The virtual address
 * Callers:
 *   - Many
 **/
vaddr_t
vmm_phys_to_virt ( paddr_t phys_addr )
{
	return _vmm_dispatch_phys_to_virt ( phys_addr );
}
