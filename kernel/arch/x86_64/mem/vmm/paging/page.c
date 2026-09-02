#include "flags.h"
#include "map.h"
#include <string.h>

/* Macro used to invalidate the Translation Lookaside Buffer (TLB) */
#define INVLPG(ADDR) \
	__asm__ volatile ( \
		"invlpg (%0)" \
		: \
		: "r" ( ADDR ) \
		: "memory" \
	);

/**
 * VMM paging internal walker
 * Private helper
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `ADDR_SPACE* space`: Given address space
 *   2. `vaddr_t virt_addr`: Virtual address
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `vaddr_t hhdm_offset`: HHDM offset used to convert physical to virtual address
 *   5. `struct page_walk* walk`: Walker context struct
 *   6. `int create`: Can new pages be allocated?
 * Callers:
 *   - Internal paging functions
 *
 * Signature macro
 **/
#define PAGING_WALK \
	kerr_t __paging_walk ( \
		ADDR_SPACE* space, \
		vaddr_t virt_addr, \
		vaddr_t hhdm_offset, \
		struct* page_walk walk, \
		bool create \
	);

/* Page table entry */
typedef vaddr_t pte_t;

/* Page table structure */
struct page_table
{
	pte_t entries[PAGE_ENTRIES];
} __attribute__ ( ( packed ) );

/* Walker context */
struct page_walk
{
	struct page_table* pml4;
	struct page_table* pdpt;
	struct page_table* pd;
	struct page_table* pt;
};

/* Prototypes */
static PAGING_WALK;

/**
 * Paging backend page mapping 
 *
 * Implements the VMM backend page mapping interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_MAP ( _paging_map )
{

	kerr_t err;
	struct page_walk walk;
	pte_t* pt_entry;

	/* Calls the walker */
	err = __paging_walk ( space, virt_addr, hhdm_offset, flags, &walk, true );
	if ( !IS_RET_OK ( err ) )
		return err;

	/* Gets the PT entry */
	pt_entry = &walk.pt->entries[PT_IDX ( virt_addr )];

	/* PT entry already set */
	if ( IS_PRESENT ( *pt_entry ) )
		return KERR_REXIST;

	/* Passes the physical address to PT entry with its flags */
	*pt_entry = phys_addr | ( PAGE_PRESENT & flags );

	/* Invalidates any cache for this page */
	INVLPG ( virt_addr );

	/* Successful mapped */
	return KERR_OK;
}

/**
 * Paging backend page unmapping 
 *
 * Implements the VMM backend page unmapping interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_UNMAP ( _paging_unmap )
{

	kerr_t err;
	struct page_walk walk;
	pte_t* pt_entry;

	/* Calls the walker */
	err = __paging_walk ( space, virt_addr, hhdm_offset, flags, &walk, false );
	if ( !IS_RET_OK ( err ) )
		return err;

	/* Gets the entry */
	pt_entry = &walk.pt->entries[PT_IDX ( virt_addr )];

	/* PT not present (can't be unmapped) */
	if ( !IS_PRESENT ( *pt_entry ) )
		return KERR_NOTMAPPED;

	/* Fill it with zeros */
	memset ( pt_entry, 0, PAGE_SIZE );
	INVLPG ( vaddr );

	/* Correctly unmapped */
	return KERR_OK;
}

/**
 * Paging backend page remapping
 *
 * Implements the VMM backend page remapping interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_REMAP ( _paging_remap )
{

	kerr_t err;
	struct page_walk walk;
	pte_t* pt_entry;

	/* As remapping operation, walker can't allocate new pages */
	err = __paging_walk ( space, virt_addr, hhdm_offset, flags, &walk, false );
	if ( !IS_RET_OK ( err ) )
		return err;

	/* Gets the PT entry */
	pt_entry = &walk.pt->entries[PT_IDX ( virt_addr )];

	/* PT entry not set. Should be mapped first */
	if ( !IS_PRESENT ( *pt_entry ) )
		return KERR_NOTMAPPED;

	/* Passes the new physical address to PT entry with its flags */
	*pt_entry = phys_addr | ( PAGE_PRESENT & flags );
	INVLPG ( virt_addr );

	/* Successful remapped */
	return KERR_OK;
}

/**
 * Paging backend function used to unmap identity mapping
 *
 * Implements the VMM backend identity unmapping interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_UNMAP_IDENTITY ( _paging_unmap_identity )
{

	struct page_table* pml4;
	struct page_table* pdpt;
	pte_t* pml4_entry;
	pte_t* pdpt_entry;

	/* Kernel address space PML4 */
	pml4 = ( struct page_table * ) _paging_phys_to_virt ( space->pml4_phys, hhdm_offset );

	/**
	 * The identity mapping was mapped the following way:
	 *   - 1 GiB PDPT huge page
	 *   - Starting at 0x0 (0x0000000000000000)
	 *   - PLM4[0] and PDPT[0]
	 **/

	/* Get the PML4 entry */
	pml4_entry = &pml4->entries[0];

	/**
	 * If it's not present, it was already unmapped
	 * This will avoid duplicate callings
	 **/
	if ( !IS_PRESENT ( *pml4_entry ) )
		return KERR_NOTMAPPED;

	/* Get the PDPT page table */
	pdpt = ( struct page_table * ) _paging_phys_to_virt ( ADDR_MASK ( *pml4_entry ), hhdm_offset );

	/* And its entry */
	pdpt_entry = &pdpt->entries[0];

	/* The same verification as above */
	if ( !IS_PRESENT ( *pdpt_entry ) )
		return KERR_NOTMAPPED;

	/* Finally, we can zero them */
	memset ( pdpt_entry, 0, PAGE_SIZE );
	memset ( pml4_entry, 0, PAGE_SIZE );

	/* Everything's good */
	return KERR_OK;
}

/**
 * Paging backend virtual address converter to physical one
 *
 * Implements the VMM backend virtual address converter interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_VIRT_TO_PHYS ( _paging_virt_to_phys )
{

	kerr_t err;
	struct page_walk walk;
	pte_t* pt_entry;

	/* Calls the walker */
	err = __paging_walk ( space, virt_addr, hhdm_offset, flags, &walk, false );
	if ( !IS_RET_OK ( err ) )
		return ENCODE_KERR ( err );

	/* Gets the correct entry */
	pt_entry = &walk->pt->entries[PT_IDX ( virt_addr )];

	/* Masks its flags to get the physical address */
	return ( paddr_t ) ADDR_MASK ( *pt_entry );
}

/**
 * Paging backend physical address converter to virtual one
 *
 * Implements the VMM backend physical address converter interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_PHYS_TO_VIRT ( _paging_phys_to_virt )
{

	/* Gets the virtual address within the HHDM direct mapping */
	return ( vaddr_t ) ( phys_addr + hhdm_offset );
}

/**
 * Paging walker 
 *
 * Interface:
 *   - Statically define at this file
 **/
PAGING_WALK
{

	/* Can't proceed */
	if ( walk == NULL )
		return KERR_INVAL;

	/* Page table indexes */
	uint16_t pml4_idx = PML4_INDEX ( virt_addr );
	uint16_t pdpt_idx = PDPT_INDEX ( virt_addr );
	uint16_t pd_idx = PD_INDEX ( virt_addr );
	uint16_t pt_idx = PT_INDEX ( virt_addr );
	uint16_t offset = ADDR_OFFSET ( virt_addr );

	struct page_table* pml4;
	struct page_table* pdpt;
	struct page_table* pd;
	struct page_table* pt;
	pte_t* pml4_entry;
	pte_t* pdpt_entry;
	pte_t* pd_entry;
	pte_t* pt_entry;

	/* Address space's PML4 */
	pml4 = ( struct page_table * ) _paging_phys_to_virt ( space->pml4_phys, hhdm_offset );

	/* Sets PML4 virtual address to `page_walk` struct pointer */
	walk->pml4 = pml4;

	/* Loads the correct indexing PML4 entry */
	pml4_entry = &pml4->entries[pml4_idx];

	/* PDPT was already mapped */
	if ( IS_PRESENT ( *pml4_entry ) )

	    /* Masks its flags to get the PDPT address */
		pdpt = ( struct page_table * )
	        _paging_phys_to_virt ( ADDR_MASK ( *pml4_entry ), hhdm_offset );

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
			return KERR_NOTMAPPED;

		/* Allocates a new physical frame */
		paddr_t pdpt_phys = pmm_alloc_frame (  );

		/* Error allocating a physical address */
		if ( IS_KERR ( pdpt_phys ) )
			return GET_KERR ( pdpt_phys );

		/* Initializes it zeroed */
		pdpt = ( struct page_table * ) _paging_phys_to_virt ( pdpt_phys, hhdm_offset );
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
	pdpt_entry = &pdpt->entries[pdpt_idx];

	/* PD is already mapped */
	if ( IS_PRESENT ( *pdpt_entry ) )

	    /* Masks the entry flags to get the PD's address */
		pd = ( struct page_table * )
	        _paging_phys_to_virt ( ADDR_MASK ( *pdpt_entry ), hhdm_offset );

	/* PD not mapped yet */
	else
	{

		/* Stops for lookups */
		if ( !create )
			return KERR_NOTMAPPED;

		/* Allocates a new frame table for the PD */
		paddr_t pd_phys = pmm_alloc_frame (  );

		/* Error: PD was not mapped */
		if ( IS_KERR ( pd_phys ) )
			return GET_KERR ( pd_phys );

		/* Initializes it zeroed */
		pd = ( struct page_table * ) _paging_phys_to_virt ( pd_phys, hhdm_offset );
		memset ( pd, 0, sizeof ( struct page_table ) );

		/* Sets the physical address and its flags to the entry */
		*pdpt_entry = pd_phys | ( PAGE_PRESENT | PAGE_RW | PAGE_US );
	}

	/* Sets PD physical address */
	walk->pd = pd;

	/* Gets the PD entry */
	pd_entry = &pd->entries[pd_idx];

	/* PT was already mapped */
	if ( IS_PRESENT ( *pd_entry ) )

	    /* Ignores the flags */
		pt = ( struct page_table * ) _paging_phys_to_virt ( ADDR_MASK ( *pd_entry ), hhdm_offset );

	/* PT was not mapped yet */
	else
	{

		/* Stops for non-allocators callers */
		if ( !create )
			return KERR_NOTMAPPED;

		/* Allocates a new physical frame for PT */
		paddr_t pt_phys = pmm_alloc_frame (  );

		/* PT not mapped */
		if ( IS_KERR ( pt_phys ) )
			return GET_KERR ( pt_phys );

		/* Initializes it zeroed */
		pt = ( struct page_table * ) _paging_phys_to_virt ( pt_phys, hhdm_offset );
		memset ( pt, 0, sizeof ( struct page_table ) );

		/* Sets the physical address and its flags to the entry */
		*pd_entry = pt_phys | ( PAGE_PRESENT | PAGE_RW | PAGE_US );
	}

	/* Finally, sets PT physical address */
	walk->pt = pt;

	/* Everything's good */
	return KERR_OK;
}
