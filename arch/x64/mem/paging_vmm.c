#include <mem/paging.h>
#include <rks/kernel/mem/arch_vmm.h>

extern char pml4[];

static paging_t paging_ctx;
static u64 translate_flags ( vmm_map_flags_t flags );

/*
 * Generic VMM initializer
 *
 * -> void
 */
void
arch_vmm_init ( void )
{
        /* *INDENT-OFF* */

	/* Defining context for paging */
	paging_ctx = ( paging_t )
	{
		.pml4 = ( page_table_t * ) pml4,
		.phys_pml4 = ( addr_t ) pml4,
	};

        /* *INDENT-ON* */
}

/*
 * x86-64 implementation of the generic architecture map layer
 *
 * -> bool: Were the page mapped?
 * @vaddr: Virtual address to be mapped
 * @paddr: Physical address
 * @flags: Generic VMM flags
 *   Refer them at `rks/kernel/mem/vmm.h`
 */
bool
arch_vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags )
{
	/* Try to map the page */
	return map_page ( vaddr, paddr, &paging_ctx,
	                  translate_flags ( flags ) );
}

/*
 * Helper used to translate each generic VMM flag to paging specific
 *
 * -> u64: Translated flags
 * @flags: Generic flags
 */
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
		paging_flags |= PCD;

	return paging_flags;
}
