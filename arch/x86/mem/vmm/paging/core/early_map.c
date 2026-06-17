#include <rks/kernel/sections.h>
#include <rks/lib/bitwise.h>
#include <x86/const.h>
#include <x86/mem/early_alloc.h>
#include <x86/mem/vmm/paging.h>

/**
 * Early page table getter (and allocator when needed)
 * Private helper
 *
 * Return -> `paddr_t *`: Table physical address pointer
 * Arguments:
 *   1. `paddr_t* entry`: Page Table entry
 *     - It can or cannot be allocated
 * Callers:
 *   - `__early_map_page`
 * Location:
 *   - `bootstrap` section
 **/
__BOOT_TEXT static paddr_t*
__early_get_or_alloc_table ( paddr_t* entry )
{

	/* Is this entry already present? */
	if ( !( *entry & PAGE_PRESENT ) )
	{

		/* Allocates a new page for this entry */
		paddr_t phys_addr =
		        __early_phys_alloc ( PAGE_SIZE, PAGE_SIZE, FILL_ZEROS );

		/* Subtracts the virtual offset to get the physical address */
		phys_addr -= KERNEL_VIRT_OFF;

		/* Writes the physicall allocated address to the entry */
		*entry = phys_addr | PAGE_PRESENT | PAGE_RW;
	}

	/* Returns the entry pointer with no flags */
	return ( paddr_t * ) ( *entry & PAGE_ADDR_MASK );
}

/**
 * Early huge 2MiB page mapper
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `vaddr_t* pml4`: PML4 physical address pointer
 *   2. `vaddr_t virt_addr`: Virtual address to be mapped
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `u32 flags`: Just the flags for the page
 * Callers:
 *   - `__early_map_range`
 * Location:
 *   - `bootstrap` section
 **/
__BOOT_TEXT static void
__early_map_page ( paddr_t* pml4, vaddr_t virt_addr, paddr_t phys_addr,
                   u32 flags )
{

	/* PDPT */
	struct page_table* pdpt =
	        __early_get_or_alloc_page ( &pml4[PML4_IDX ( virt_addr )] );

	/* PD */
	struct page_table* pd =
	        __early_get_or_alloc_page ( &pdpt[PDPT_IDX ( virt_addr )] );

	/* Fills PD */
	pd[PD_IDT ( virt_addr )] = phys_addr | ( flags | PAGE_HUGE );
}

/**
 * Early page range mapper
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `vaddr_t* pml4`: PML4 physical address pointer
 *   2. `vaddr_t virt_addr`: Virtual address to be mapped
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `size_t size`: Range size
 *   5. `u32 flags`: Just the flags for the page
 * Callers:
 *   - VMM internals
 * Location:
 *   - `bootstrap` section
 **/
__BOOT_TEXT void
_early_map_range ( paddr_t* pml4, vaddr_t virt_addr, paddr_t phys_addr,
                   size_t size, u32 flags )
{
	/* Guarantees the size is aligned up */
	size = ALIGN_UP ( size, 0x200000 );

        /* Aligns the physical base address down */
	phys_addr = ALIGN_DOWN ( phys_addr, 0x200000 );

	/* Iterates for each page within the size */
	for ( size_t offset = 0; offset < size; offset += PAGE_SIZE )
		__early_map_page ( pml4, virt_addr + offset, phys_addr + offset,
	                           flags );
}
