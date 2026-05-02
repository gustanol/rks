#include <rks/kernel/mem/vmm.h>
#include <mem/arch_vmm.h>

/*
 * Virtual Memory Manager initializer
 * It should be called only once at main function
 *
 * -> void
 */
void
vmm_init ( void )
{
	/* Achitecture-specific context defining */
	_arch_vmm_init (  );

	/* The kernel size to be used to map it entirely */
	u64 kernel_size = ( paddr_t ) & kernel_end - ( paddr_t ) & kernel_start;

	/* Map all the kernel code to its virtual base */
	vmm_map_range ( ( vaddr_t ) KERNEL_VIRT_BASE,
	                ( paddr_t ) kernel_start, kernel_size,
	                VMM_RW | VMM_PRESENT );

	size_t total_memory = pmm_get_total_memory (  );

	/* Create a physical mapping of all physical RAM at PHYSMAP BASE */
	vmm_map_range ( ( vaddr_t ) KERNEL_PHYSMAP_BASE, 0x0, total_memory,
	                VMM_RW | VMM_PRESENT );
}

/*
 * Helper used to map a virtual page
 * It calls the architecture-dependent code to do that
 *
 * -> bool: Was it mapped?
 * @vaddr: Virtual address
 * @paddr: Physical address
 * @flags: Modifying flags
 */
bool
vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags )
{
	return _arch_vmm_map ( vaddr, paddr, flags );
}

/*
 * Middle helper used to map a range of physical memory
 *
 * -> bool: Was it mapped?
 * @vaddr: Virtual address
 * @paddr: Physical address
 */
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
