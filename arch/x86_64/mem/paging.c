#include <mem/paging.h>
#include <rks/lib/string.h>
#include <rks/kernel/klog/print.h>

/* Invalidate the Translation Lookaside Buffer (TLB) */
#define INVLPG(ADDR) \
	__asm__ volatile ( \
                "invlpg (%0)" \
                : \
                : "r" ( ADDR ) \
                : "memory" \
        );


/*
 * Map a new page
 *
 * -> bool: True if everything gone right
 *
 * @vaddr: Virtual address
 * @paddr: Physical address
 * @paging_ctx: Useful struct containing PML4 context
 * @flags: Translated flags
 */
bool
map_page ( vaddr_t vaddr, paddr_t paddr, paging_t* paging_ctx, u64 flags )
{
	/* Index of each Page Table */
	u16 pml4_idx = PML4_INDEX ( vaddr );
	u16 pdpt_idx = PDPT_INDEX ( vaddr );
	u16 pd_idx = PD_INDEX ( vaddr );
	u16 pt_idx = PT_INDEX ( vaddr );

	/* Each page table */
	page_table_t* pdpt,
	 *pd,
	 *pt;

	/* Entry offsets gotten from the virtual address */
	pte_t* pml4_entry,
	 *pdpt_entry,
	 *pd_entry,
	 *pt_entry;

	/*
	 * Get the PML4 entry from the virtual address
	 *
	 * It points (or not) to a PDPT table
	 */
	pml4_entry = &paging_ctx->pml4->entries[pml4_idx];

	/* PDPT not mapped */
	if ( !( *pml4_entry & PPRESENT ) )
	{
		/* Allocate a new physical frame */
		addr_t pdpt_phys = pmm_alloc_frames ( 1 );

		/* PDPT not mapped */
		if ( !pdpt_phys )
			return false;

		/* Initialize it with zero */
		pdpt = ( page_table_t * ) pdpt_phys;
		memset ( pdpt, 0, sizeof ( page_table_t ) );

		/* Set the PDPT physical address and the specified flags */
		*pml4_entry = pdpt_phys | PPRESENT | PRW | PUS;
	}

	/* PDPT already mapped */
	else
	{
		/* PDPT = &PML4[INDEX] */
		pdpt = ( page_table_t * ) ( *pml4_entry );
	}

	/* PDPT[i] -> PD */
	pdpt_entry = &pdpt->entries[pdpt_idx];

	/* PD not mapped yet */
	if ( !( *pdpt_entry & PPRESENT ) )
	{
		/* Map it */
		addr_t pd_phys = pmm_alloc_frames ( 1 );

		/* Mapping gone wrong */
		if ( !pd_phys )
			return false;

		/* Get the physical address and set it with zeros */
		pd = ( page_table_t * ) pd_phys;
		memset ( pd, 0, sizeof ( page_table_t ) );

		/* Set the physical address of PD */
		*pdpt_entry = pd_phys | PPRESENT | PRW | PUS;
	}

	/* PD already mapped */
	else
	{
		pd = ( page_table_t * ) ( *pdpt_entry );
	}

	/* PD[i] -> PT */
	pd_entry = &pd->entries[pd_idx];

	/* PT not mapped yet */
	if ( !( *pd_entry & PPRESENT ) )
	{
		/* Map it */
		addr_t pt_phys = pmm_alloc_frames ( 1 );

		/* Not mapped */
		if ( !pt_phys )
			return false;

		/* Initialize this PT and set it with zeros */
		pt = ( page_table_t * ) pt_phys;
		memset ( pt, 0, sizeof ( page_table_t ) );

		/* Set the physical address of this PT */
		*pd_entry = pt_phys | PPRESENT | PRW | PUS;
	}

	/* PT already mapped */
	else
	{
		pt = ( page_table_t * ) ( *pd_entry );
	}

	/* Finally, the page */
	pt_entry = &pt->entries[pt_idx];

	/* Pass the physical address */
	*pt_entry = paddr | flags;

	/* Invalidate any cache for this page */
	INVLPG ( vaddr );

	return true;
}

/*
 * Unmap a page
 *
 * -> bool: 1 on success
 * @vaddr: Virtual address
 * @paging_ctx: Struct pointer containing a pointer to PML4
 */
bool
unmap_page ( vaddr_t vaddr, paging_t* paging_ctx )
{
	/* Indexes from the virtual address */
	u16 pml4_idx = PML4_INDEX ( vaddr );
	u16 pdpt_idx = PDPT_INDEX ( vaddr );
	u16 pd_idx = PD_INDEX ( vaddr );
	u16 pt_idx = PT_INDEX ( vaddr );

	/* Each page table */
	page_table_t* pdpt,
	 *pd,
	 *pt;

	/* Entry offsets gotten from the virtual address */
	pte_t* pml4_entry,
	 *pdpt_entry,
	 *pd_entry,
	 *pt_entry;

	/* Pass through each hierarchy layer 'til reach the PT */
	pml4_entry = &paging_ctx->pml4->entries[pml4_idx];
	if ( !( *pml4_entry & PPRESENT ) )
		return false;

	/* PDPT table */
	pdpt = ( page_table_t * ) ( *pml4_entry );
	pdpt_entry = &pdpt->entries[pdpt_idx];

	if ( !( *pdpt_entry & PPRESENT ) )
		return false;
	/* PDPT table */

	/* PD table */
	pd = ( page_table_t * ) ( *pdpt_entry );
	pd_entry = &pd->entries[pd_idx];

	if ( !( *pd_entry & PPRESENT ) )
		return false;
	/* PD table */

	/* PT table */
	pt = ( page_table_t * ) ( *pd_entry );
	pt_entry = &pt->entries[pt_idx];

	if ( !( *pt_entry & PPRESENT ) )
		return false;
	/* PT table */

	/* Zero it */
	memset ( pt_entry, 0, PAGE_SIZE );

	/* Invalidate cache */
	INVLPG ( vaddr );

	return 1;
}

/*
 * Helper to translate virtual addresses to physical ones
 *
 * -> paddr_t: The physical translated address
 * @vaddr: The virtual address
 * @paging_ctx: Pointer to the useful paging context structure
 */
paddr_t
get_phys ( vaddr_t vaddr, paging_t* paging_ctx )
{
	/* Indexes from the virtual address */
	u16 pml4_idx = PML4_INDEX ( vaddr );
	u16 pdpt_idx = PDPT_INDEX ( vaddr );
	u16 pd_idx = PD_INDEX ( vaddr );
	u16 pt_idx = PT_INDEX ( vaddr );
	u16 offset = ADDR_OFFSET ( vaddr );

	/* Each page table */
	page_table_t* pdpt,
	 *pd,
	 *pt;

	/* Entries */
	pte_t* pml4_entry,
	 *pdpt_entry,
	 *pd_entry,
	 *pt_entry;

	/* Go through each hierarchy layer 'til reach the PT */
	pml4_entry = &paging_ctx->pml4->entries[pml4_idx];
	if ( !( *pml4_entry & PPRESENT ) )
		return false;

	/* PDPT */
	pdpt = ( page_table_t * ) ( *pml4_entry );
	pdpt_entry = &pdpt->entries[pdpt_idx];

	if ( !( *pdpt_entry & PPRESENT ) )
		return false;
	/* PDPT */

	/* PD */
	pd = ( page_table_t * ) ( *pdpt_entry );
	pd_entry = &pd->entries[pd_idx];

	if ( !( *pd_entry & PPRESENT ) )
		return false;
	/* PD */

	/* PT */
	pt = ( page_table_t * ) ( *pd_entry );
	pt_entry = &pt->entries[pt_idx];

	if ( !( *pt_entry & PPRESENT ) )
		return false;
	/* PT */

	/* Align the address by 4096 to remove the flags */
	return ( ( ALIGN ( *pt_entry, 0xC ) ) + offset );

}

/*
 * Debug function to list the page hierarchy of the passed virtual address
 *
 * -> void
 * @vaddr: The virtual address
 * @paging_ctx: Pointer to paging context struct
 */
void
dump_tables ( vaddr_t vaddr, paging_t* paging_ctx )
{
	kprintf ( "Page tables for %llx:\n[ P = Present | NP = Not Present ]\n",
	          vaddr );

	/* PML4 */
	u16 pml4_idx = PML4_INDEX ( vaddr );
	pte_t pml4_entry = paging_ctx->pml4->entries[pml4_idx];

	kprintf ( "  PML4[%d] = %llx [%s]\n", pml4_idx, pml4_entry,
	          pml4_entry & PPRESENT ? "P" : "NP" );

	if ( !( pml4_entry & PPRESENT ) )
		goto print_offset;
	/* PML4 */

	/* PDPT table */
	u16 pdpt_idx = PDPT_INDEX ( vaddr );
	page_table_t* pdpt = ( page_table_t * ) ( pml4_entry );
	pte_t pdpt_entry = pdpt->entries[pdpt_idx];

	kprintf ( "  PDPT[%d] = %llx [%s]\n", pdpt_idx, pdpt_entry,
	          pdpt_entry & PPRESENT ? "P" : "NP" );

	if ( !( pdpt_entry & PPRESENT ) )
		goto print_offset;
	/* PDPT table */

	/* PD table */
	u16 pd_idx = PD_INDEX ( vaddr );
	page_table_t* pd = ( page_table_t * ) ( pdpt_entry );
	pte_t pd_entry = pd->entries[pd_idx];

	kprintf ( "  PD[%d] = %llx [%s]\n", pd_idx, pd_entry,
	          pd_entry & PPRESENT ? "P" : "NP" );

	if ( !( pd_entry & PPRESENT ) )
		goto print_offset;
	/* PD table */

	/* PT table */
	u16 pt_idx = PT_INDEX ( vaddr );
	page_table_t* pt = ( page_table_t * ) ( pd_entry );
	pte_t pt_entry = pt->entries[pt_idx];

	kprintf ( "  PT[%d] = %llx [%s]\n", pt_idx, pt_entry,
	          pt_entry & PPRESENT ? "P" : "NP" );
	/* PT table */

      print_offset:
	{
		u16 offset = ADDR_OFFSET ( vaddr );

		kprintf ( "  Offset = %llx\n", offset );
	}
}
