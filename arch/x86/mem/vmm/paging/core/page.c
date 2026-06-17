#include <rks/kernel/klog/print.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/lib/bitwise.h>
#include <rks/lib/string.h>
#include <x86/mem/vmm/paging.h>

/* Macro used to invalidate the Translation Lookaside Buffer (TLB)
 * =============================================================== */
#define INVLPG(ADDR) \
	__asm__ volatile ( \
                "invlpg (%0)" \
                : \
                : "r" ( ADDR ) \
                : "memory" \
        );

struct page_walk
{
	struct page_table* pml4;
	struct page_table* pdpt;
	struct page_table* pd;
	struct page_table* pt;
};

static void __paging_walk ( ADDRESS_SPACE * space, vaddr_t virt_addr,
                            paddr_t phys_addr, vaddr_t hhdm_offset,
                            struct* page_walk walk, int create );

/**
 * x86 VMM paging mapping
 * Subsystem backend function
 *
 * Return -> `int`:
 *   - 1: Everything went fine
 *   - 0: Not good at all
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the page in
 *   2. `vaddr_t virt_addr`: Virtual address
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `vaddr_t hhdm_offset`: Kernel HHDM offset
 * Callers:
 *   - VMM implementations
 **/
int
_paging_map ( ADDR_SPACE* space, vaddr_t virt_addr, paddr_t phys_addr,
              vaddr_t hhdm_offset, u32 flags )
{
	/* Stack-allocated `page_walk` struct */
	struct page_walk walk;

	/* Calls the walker */
	__paging_walk ( space, virt_addr, phys_addr, hhdm_offset, flags, &walk,
	                1 );

	/* Gets the PT entry */
	pte_t* pt_entry = &walk->pt->entries[PT_IDX ( virt_addr )];

	/* Passes the physical address to PT entry with its flags */
	*pt_entry = phys_addr | flags;

	/* Invalidates any cache for this page */
	INVLPG ( virt_addr );

	/* Successful mapped */
	return 1;
}

/**
 * x86 VMM paging unmapping
 * Subsystem backend function
 *
 * Return -> `int`
 *   - 1: Only on address unmapping
 *   - 0: On failure (address not reached)
 * Arguments:
 *   1. `ADDR_SPACE* space`: Addess space the page table should be unmapped from
 *   2. `vaddr_t virt_addr`: Page virtual address
 *   3. `vaddr_t hhdm_offset`: Kernel HHDM offset
 * Callers:
 *   - VMM implementations
 **/
int
_paging_unmap ( ADDR_SPACE* space, vaddr_t virt_addr, vaddr_t hhdm_offset )
{
	/* Stack-allocated `page_walk` struct */
	struct page_walk walk;

	/* Calls the walker */
	__paging_walk ( space, virt_addr, phys_addr, hhdm_offset, flags, &walk,
	                0 );

	if ( walk->pt == NULL )
		return 0;

	pte_t* pt_entry & walk->pt->entries[PT_IDX ( virt_addr )];

	/* Sets the entry with zeros */
	memset ( pt_entry, 0, PAGE_SIZE );

	/* Invalidates cache */
	INVLPG ( vaddr );

	/* Correctly unmapped */
	return 1;
}

/**
 * Physical address conversor
 * Subsystem backend function
 *
 * Return -> `paddr_t`: The converted physical address
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to get the virtual address from
 *   2. `vaddr_t virt_addr`: Virtual address to be converted
 *   3. `vaddr_t hhdm_offset`: HHDM offset to be used
 * Callers:
 *   - VMM implementations
 **/
paddr_t _paging_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr,
                               vaddr_t hhdm_offset );
{
	/* Stack-allocated `page_walk` struct */
	struct page_walk walk;

	/* Calls the walker */
	__paging_walk ( space, virt_addr, phys_addr, hhdm_offset, flags, &walk,
	                0 );

	if ( walk->pt == NULL )
		return 0;

	pte_t* pt_entry & walk->pt->entries[PT_IDX ( virt_addr )];

	return _paging_phys_to_virt ( ADDR_MASK ( *pt_entry ), hhdm_offset );
}

/**
 * Virtual address conversor
 * Subsystem backend function
 *
 * Return -> `vaddr_t`: The converted virtual address
 * Arguments:
 *   1. `paddr_t phys_addr`: Physical address to be converted
 *   2. `vaddr_t hhdm_offset`: HHDM offset to be used
 * Callers:
 *   - VMM implementations
 **/
vaddr_t
_paging_phys_to_virt ( paddr_t phys_addr, vaddr_t hhdm_offset )
{
	/* Gets the virtual address within the HHDM direct mapping */
	return ( vaddr_t ) ( phys_addr + hhdm_offset );
}

/**
 * VMM paging internal walker
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* space`: Given address space
 *   2. `vaddr_t virt_addr`: Virtual address
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `vaddr_t hhdm_offset`: HHDM offset used to convert physical to virtual address
 *   5. `struct page_walk* walk`: Walker context struct
 *   6. `int create`: Can new pages be allocated?
 * Callers:
 *   - Internal paging functions
 **/
void
__paging_walk ( ADDR_SPACE* space, vaddr_t virt_addr,
                paddr_t phys_addr, vaddr_t hhdm_offset,
                struct* page_walk walk, int create )
{
	/* Page table indexes */
	u16 pml4_idx = PML4_INDEX ( virt_addr );
	u16 pdpt_idx = PDPT_INDEX ( virt_addr );
	u16 pd_idx = PD_INDEX ( virt_addr );
	u16 pt_idx = PT_INDEX ( virt_addr );
	u16 offset = ADDR_OFFSET ( virt_addr );

	/* Address space's PML4 */
	struct page_table* pml4 =
	        ( struct page_table * ) _paging_phys_to_virt ( space->pml4_phys,
	                                                       hhdm_offset );

	/* Page Directory Pointer Table */
	struct page_table* pdpt;

	/* Page Directory */
	struct page_table* pd;

	/* Page Table */
	struct page_table* pt;

	/* Sets PML4 virtual address to `page_walk` struct pointer */
	walk->pml4 = pml4;

	/* Loads the correct indexing PML4 entry */
	pte_t* pml4_entry = &pml4->entries[pml4_idx];

	/* PDPT was already mapped */
	if ( *pml4_entry & PAGE_PRESENT )

	        /* Masks its flags to get the PDPT address */
		pdpt = ( struct page_table * )
	                _paging_phys_to_virt ( ADDR_MASK ( *pml4_entry ),
	                                       hhdm_offset );

	/* PDPT was not mapped */
	else
	{

		/**
                 * Logically, if PDPT was not mapped and this calling
                 * Is just for lookup, we can't proceed
                 *
                 * In this case, nothing else can be done
                 **/
		if ( !create )
			return;

		/* Allocates a new physical frame */
		paddr_t pdpt_phys = pmm_alloc_frame (  );

		/* Error allocating a physical address */
		if ( !pdpt_phys )

			/*
			   k_panic ("error allocating a physical frame");
		         */

			return;

		/* Initializes it zeroed */
		pdpt = ( struct page_table * )
		        _paging_phys_to_virt ( pdpt_phys, hhdm_offset );
		memset ( pdpt, 0, sizeof ( struct page_table ) );

		/**
                 * Sets the PDPT allocated physical address to the PML4 entry
                 *
                 * Note: We already got the PDPT address
                 * Modifying this entry won't change it
                 **/
		*pml4_entry = pdpt_phys | ( PAGE_PRESENT | PAGE_RW | PAGE_US );
	}

	/* Sets PDPT virtual address */
	walk->pdpt = pdpt;

	/* Gets the PDPT entry */
	pte_t* pdpt_entry = &pdpt->entries[pdpt_idx];

	/* PD is already mapped */
	if ( *pdpt_entry & PAGE_PRESENT )

	        /* Masks the entry flags to get the PD's address */
		pd = ( struct page_table * )
	                _paging_phys_to_virt ( ADDR_MASK ( *pdpt_entry ),
	                                       hhdm_offset );

	/* PD not mapped yet */
	else
	{
		/* Stops for lookups */
		if ( !create )
			return;

		/* Allocates a new frame table for the PD */
		paddr_t pd_phys = pmm_alloc_frame (  );

		/* Error: PD was not mapped */
		if ( !pd_phys )

			/*
			   k_panic();
		         */

			return;

		/* Initializes it zeroed */
		pd = ( struct page_table * ) _paging_phys_to_virt ( pd_phys,
		                                                    hhdm_offset );
		memset ( pd, 0, sizeof ( struct page_table ) );

		/* Sets the physical address and its flags to the entry */
		*pdpt_entry = pd_phys | ( PAGE_PRESENT | PAGE_RW | PAGE_US );
	}

	/* Sets PD physical address */
	walk->pd = pd;

	/* Gets the PD entry */
	pte_t* pd_entry = &pd->entries[pd_idx];

	/* PT was already mapped */
	if ( *pd_entry & PAGE_PRESENT )

	        /* Ignores the flags */
		pt = ( struct page_table * )
	                _paging_phys_to_virt ( ADDR_MASK ( *pd_entry ),
	                                       hhdm_offset );

	/* PT was not mapped yet */
	else
	{
		/* Stops for non-allocators callers */
		if ( !create )
			return;

		/* Allocates a new physical frame for PT */
		paddr_t pt_phys = pmm_alloc_frame (  );

		/* PT not mapped */
		if ( !pt_phys )

			/*
			   k_panic();
		         */

			return;

		/* Initializes it zeroed */
		pt = ( struct page_table * ) _paging_phys_to_virt ( pt_phys,
		                                                    hhdm_offset );
		memset ( pt, 0, sizeof ( struct page_table ) );

		/* Sets the physical address and its flags to the entry */
		*pd_entry = pt_phys | ( PAGE_PRESENT | PAGE_RW | PAGE_US );
	}

	/* Finally, sets PT physical address */
	walk->pt = pt;
}
