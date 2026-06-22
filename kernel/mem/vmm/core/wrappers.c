#include <rks/kernel/mem/vmm/addr_space.h>
#include <rks/kernel/mem/vmm/map.h>
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
	return ( ADDR_SPACE * ) _vmm_dispatch_create_space (  );
}

/**
 * Public address space destroyer
 * Public wrapper
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* space`: Space to be destroyed
 * Callers:
 *   - Many
 **/
void
vmm_destroy_space ( ADDR_SPACE* space )
{
	_vmm_dispatch_destroy_space ( space );
}

/**
 * Public address space switcher
 * Public wrapper
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to be switched to
 * Callers:
 *   - Many
 **/
void
vmm_switch_to_space ( ADDR_SPACE* space )
{
	_vmm_dispatch_switch_to_space ( space );
}

/**
 * Public virtual memory mapper
 * Public wrapper
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
 *   - Many
 **/
int
vmm_map ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, u32 flags )
{
	return _vmm_dispatch_map ( space, virt_addr, phys_addr, flags );
}

/**
 * Public virtual memory unmapper
 * Public wrappers
 *
 * Return -> `int`:
 *   - 1: Successfully unmapped
 *   - 0: Not unmapped
 *     - Probably, any upper layer were not mapped yet
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to unmap the address from
 *   2. `vaddr_t virt_addr`: Virtual address
 * Callers:
 *   - Many
 **/
int
vmm_unmap ( ADDR_SPACE* space, vaddr_t virt_addr )
{
	return _vmm_dispatch_unmap ( space, virt_addr );
}

/**
 * Public virtual memory remapper
 * Public wrapper
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
 *   - Many
 *
 * - Should be used to map to already-mapped addresses
 **/
int
vmm_remap ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr, u32 flags )
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
	return ( paddr_t ) _vmm_dispatcher_virt_to_phys ( space, virt_addr );
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
	return ( vaddr_t ) _vmm_dispatcher_phys_to_virt ( phys_addr );
}
