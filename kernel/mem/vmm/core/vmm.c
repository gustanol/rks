#include <arch/const.h>
#include <arch/mem/arch_vmm.h>
#include <arch/mem/early_alloc.h>
#include <rks/kernel/mem/vmm.h>
#include <rks/kernel/subsys/registry.h>

/**
 * Virtual Memory Manager initializer
 * Public subsystem initializer
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `rks_main`
 **/
void
vmm_init ( void )
{

	/* VMM global state structure allocation */
	g_vmm = ( struct vmm * ) _early_alloc ( sizeof ( struct vmm ), 0x10,
	                                        FILL_ZEROS );

	/* Gets the registered VMM backend (architecture-dependent) */
	struct subsys_backend* backend =
	        subsys_find_backend ( SUBSYSTEM_TAG_VMM );

	/*
	   if (!backend)
	   k_panic("no registered backend");
	 */

	/*
	   if (!backend->init)
	   k_panic("no registered backend");
	 */

	/* Initializes the backend */
	backend->init ( g_vmm->kernel.addr_space );

	/* Sets the registered operation functions as default */
	g_vmm->ops = ( struct vmm_ops * ) backend->ops;

	/* Maps all physical memory to higher half */
	g_vmm->ops->early_build_hhdm ( g_vmm->kernel.addr_space );

	/**
         * Sets the HHDM offset after all
         *
         * Notice we could've done this before
         * Doing now is just a convention because the HHDM was fully set
         **/
	g_vmm->hhdm_offset = HHDM_BASE;

	/* Now we've got HHDM mapped, there's no reason to keep identity mapping */
	g_vmm->ops->early_identity_unmap ( g_vmm->kernel.addr_space );

	/* Sets the kernel limit
	 * ===================== */
	g_vmm->kernel.virt_base = KERNEL_START;
	g_vmm->kernel.virt_end = KERNEL_END;

	/* Every is ready from now on */
	g_vmm->ready = 1;
}

/**
 * Helper used to map a virtual page
 * It calls the architecture-dependent code to do that
 *
 * -> bool: Was it mapped?
 * @vaddr: Virtual address
 * @paddr: Physical address
 * @flags: Modifying flags
 **/
bool
vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags )
{
	return _arch_vmm_map ( vaddr, paddr, flags );
}

/**
 * Middle helper used to map a range of physical memory
 *
 * -> bool: Was it mapped?
 * @vaddr: Virtual address
 * @paddr: Physical address
 **/
bool
vmm_map_range ( vaddr_t vaddr, paddr_t paddr, size_t size,
                vmm_map_flags_t flags )
{
	/*
	 * Get the needed amount of pages
	 *
	 * Notice we sum `PAGE_SIZE - 1` to the size before dividing it by the value of PAGE_SIZE
	 *
	 * This will guarantee we will get the exact right amound of pages we need
	 * E.G.: size = 1; ( 1 + 4096 - 1) / 4096
	 *  - Without it, we will get 0 (We can't simply sum one in cases)
	 */
	size_t pages = ( size + PAGE_SIZE - 1 ) / PAGE_SIZE;

	/* Map each of them */
	for ( size_t i = 0; i < pages; i++ )
	        /* Map always the current page (i * PAGE_SIZE) */
		if ( !vmm_map
		     ( vaddr + ( i * PAGE_SIZE ),
		       paddr + ( i * PAGE_SIZE ), flags ) )
			return false;

	/* All pages successfully mapped */
	return true;
}

/*
 * Helper used to unmap a virtual page
 *
 * -> bool: Was it good?
 * @vaddr: The virtual address
 */
bool
vmm_unmap ( vaddr_t vaddr )
{
	return _arch_vmm_unmap ( vaddr );
}

/*
 * Helper used to print the tables of a virtual address
 *
 * -> void
 * @vaddr: Virtual address
 */
void
vmm_dump_tables ( vaddr_t vaddr )
{
	_arch_vmm_dump_tables ( vaddr );
}
