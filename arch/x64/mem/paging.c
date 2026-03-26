#include <mem/paging.h>
#include <rks/kernel/mem/arch_vmm.h>

extern char pml4[];

static paging_t paging_ctx;
static u64 translate_flags ( vmm_map_flags_t flags );

void
arch_vmm_init (  )
{
	/* Defining context for paging */
	paging_ctx = ( paging_ctx_t )
	{
		.pml4 = ( page_table_t * ) pml4;
		.phys_pml4 = ( addr_t ) pml4;
	};
}

/*
 * x86-64 implementation of the generic architecure map layer
 *
 * -> bool: Were the page mapped?
 * @vaddr: Virtual address to be mapped
 * @paddr: Physical address
 * @flags: Just flags
 *   Refer them at `rks/kernel/mem/vmm.h`
 */
bool
arch_vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags )
{
	u64 paging_flags = translate_flags ( flags );

	/* Index of each Page Table */
	u16 pml4_idx = PML4_INDEX ( vaddr );
	u16 pdpt_idx = PDPT_INDEX ( vaddr );
	u16 pd_idx = PD_INDEX ( vaddr );
	u16 pt_idx = PT_INDEX ( vaddr );

	/* Each page table */
	page_table* pdpt,
	  pd,
	  pt;

	/* Entry offsets gotten from the virtual address */
	pte_t* pml4_entry,
	  pdpt_entry,
	  pd_entry,
	  pt_entry;

	/*
	 * Get the current PML4 entry
	 *
	 * It points (or not) to a PDPT table
	 */
	pml4_entry = &paging_ctx.pml4->entries[pml4_idx];

	/* PDPT not mapped */
	if ( !( *pml4_entry & PPRESENT ) )
	{
		/* Allocate a new physical frame */
		addr_t pdpt_phys = pmm_alloc_frames ( 1 );

		/* PDPT not mapped */
		if ( IS_NULL ( pdpt_phys ) )
			return false;

		/* Initialize it with zero */
		pdpt = ( page_table_t * ) pdpt_phys;
		memset ( pdpt, 0, sizeof ( page_table_t ) );

		pml4_entry = pdpt_phys | PPRESENT | PRW | PUS;
	}

	/* PDPT already mapped */
	else
	{
		pdpt = ( page_table_t * ) ( *pml4_entry );
	}

	/* PDPT[i] -> PD */
	pdpt_entry = &pdpt->entries[pdpt_idx];

	/* PD not mapped yet */
	if ( !( *pdpt_entry & PPRESENT ) )
	{
		addr_t pd_phys = pmm_alloc_frames ( 1 );

		if ( IS_NULL ( pd_phys ) )
			return false;

		pd = ( page_table_t * ) pd_phys;
		memset ( pd, 0, sizeof ( page_table_t ) );

		pd_entry = pd_phys | PPRESENT | PRW | PUS;
	}

	/* PD already mapped */
	else
	{
		pd = ( page_table_t * ) ( *pdpt_entry );
	}

	pd_entry = &pd->entries[pd_idx];

	if ( !( *pd_entry & PPRESENT ) )
	{
		addr_t pt_phys = pmm_alloc_frames ( 1 );

		if ( IS_NULL ( pt_phys ) )
			return false;

		pt = ( page_table_t * ) pt_phys;
		memset ( pt, 0, sizeof ( page_table_t ) );

		pt_entry = pt_phys | PPRESENT | PRW | PUS;
	}

	else
	{
		pt = ( page_table_t * ) ( *pd_entry );
	}

	pt_entry = &pt->entries[pt_idx];
	pt_entry = paddr | paging_flags;

	__asm__ volatile ( "invlpg (%0)"::"r" ( vaddr ):"memory" );

	return true;
}

static u64
translate_flags ( vmm_map_flags_t flags )
{
	u64 paging_flags = 0;

	/* Page present */
	if ( flags & VMM_PRESENT )
		paging_flags |= PPRESENT;

	/* Read-write */
	if ( flags & VMM_RW )
		paging_flags |= PRW;

	/* Read/Write permissions */
	if ( flags & VMM_USER )
		paging_flags |= PUS;

	/* Cacheable page */
	if ( flags & VMM_NOCACHE )
		paging_falgs |= PCD;

	return paging_flags;
}
