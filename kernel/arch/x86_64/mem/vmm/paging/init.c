#include "addr_space.h"
#include "early_map.h"
#include "map.h"
#include <arch/const.h>
#include <vmm/backend.h>

/* Static prototypes */
static VMM_BACKEND_INIT ( __paging_init );
static VMM_BACKEND_MAP_HHDM ( _paging_map_hhdm );

/* Paging backend operations setting */
static const struct vmm_ops paging_ops = {
	.early_build_hhdm = _paging_map_hhdm,
	.unmap_identity = _paging_unmap_identity,
	.create_space = _paging_create_space,
	.destroy_space = _paging_destroy_space,
	.switch_to_space = _paging_switch_to_space,
	.map = _paging_map,
	.unmap = _paging_unmap,
	.remap = _paging_remap,
	.virt_to_phys = _paging_virt_to_phys,
	.phys_to_virt = _paging_phys_to_virt,
};

/* VMM backend context */
static const struct vmm_backend paging_backend = {

	/* *INDENT-OFF* */

	.base = {
		.name = "x86_vmm_paging",
		.tag = SUBSYS_TAG_VMM,
	},

	/* *INDENT-ON* */

	.init = __paging_init,
	.ops = &paging_ops,
};

/* Backend registration */
REGISTER_VMM_BACKEND ( paging_backend );

/**
 * Paging backend initialization
 *
 * Implements the VMM backend initialization interface
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_INIT ( __paging_init )
{

	/* Stack variable to store the CR3 (PML4) value */
	paddr_t cr3;

	/* *INDENT-OFF* */

	/* Reads CR3 and passes its value to the variable */
	__asm__ volatile (
		"movq %%cr3, %0"
		: "=r" ( cr3 )
	);

	/* *INDENT-ON* */

	/* Updates the kernel address space */
	kernel_addr_space->pml4_phys = ( paddr_t ) cr3;
}

/**
 * Paging backend HHDM mapper
 *
 * Implements the VMM backend HHDM mapper
 *
 * Interface:
 *   - `kernel/mem/vmm/include/vmm/backend.h`
 **/
VMM_BACKEND_MAP_HHDM ( _paging_map_hhdm );
{

	/* Memory size to be mapped */
	size_t memory_size = pmm_get_total_memory (  );

	/* Map the whole memory size to the HHDM base */
	_early_map_range ( &kernel_addr_space->pml4_phys,
	    ( vaddr_t ) HHDM_BASE, ( paddr_t ) 0x0, memory_size, ( PAGE_PRESENT | PAGE_WRITE ) );
}
