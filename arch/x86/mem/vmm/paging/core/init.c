#include <vmm/backend.h>
#include <x86/const.h>
#include <x86/mem/vmm/paging/internal/ops.h>

/* Static prototypes
 * ================= */
void __paging_init ( ADDR_SPACE * kernel_address_space );
void _paging_build_hhdm ( ADDR_SPACE * kernel_address_space );
void _paging_identity_unmap ( ADDR_SPACE * kernel_address_space );

/* Paging backend operations setting
 * ================================= */
static const struct vmm_ops paging_ops = {
	.early_build_hhdm = _paging_build_hhdm,
	.early_identity_unmap = _paging_identity_unmap,
	.create_space = _paging_create_space,
	.destroy_space = _paging_destroy_space,
	.switch_to_space = _paging_switch_to_space,
	.map = _paging_map,
	.unmap = _paging_unmap,
	.remap = _paging_remap,
	.virt_to_phys = _paging_virt_to_phys,
	.phys_to_virt = _paging_phys_to_virt,
	.flush_tlb = _paging_flush_tlb,
};

/* Backend registration */
REGISTER_VMM_BACKEND ( "x86_paging_vmm", __paging_init, &paging_ops );

/**
 * Paging VMM backend initializer
 * Backend init function
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* kernel_address_space`: As it suggests, it's the kernel \
 *   address space pointer to be modified
 * Callers:
 *   - `vmm_init`
 **/
void
__paging_init ( ADDR_SPACE* kernel_address_space )
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
	kernel_address_space->pml4_phys = ( paddr_t ) cr3;
}

/**
 * Memory Higher Half Direct Mapping builder
 * Backend operation
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* kernel_address_space`: Address space containing the kernel PML4
 * Callers:
 *  - VMM implementation
 **/
void
_paging_build_hhdm ( ADDR_SPACE* kernel_address_space )
{
	/* Memory size to be mapped */
	size_t memory_size = pmm_get_total_memory (  );

	/* Map the whole memory size to the HHDM base */
	_early_map_range ( &kernel_address_space->pml4_phys,
	                   ( vaddr_t ) HHDM_BASE, ( paddr_t ) 0x0, memory_size,
	                   ( PAGE_PRESENT | PAGE_WRITE ) );
}
