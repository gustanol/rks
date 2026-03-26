#include <rks/kernel/mem/vmm.h>
#include <rks/kernel/mem/arch_vmm.h>

extern void kernel_start ( void );
extern void kernel_end ( void );

static inline bool vmm_map ( vaddr_t vaddr, paddr_t paddr,
                             vmm_map_flags_t flags );
static bool vmm_map_range ( vaddr_t vaddr, paddr_t paddr,
                            size_t size, vmm_map_flags_t flags );

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
	arch_vmm_init (  );

	u64 kernel_size = ( u64 ) & kernel_end - ( u64 ) & kernel_start;

	/* vmm_map_range ( VIRTUAL_BASE, PHYSICAL_BASE, kernel_size, VMM_WRITE ); */
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
	return arch_vmm_map ( table, vaddr, paddr, flags );
}

static bool
vmm_map_range ( vaddr_t vaddr, paddr_t paddr, size_t size,
                vmm_map_flags_t flags )
{
	size_t pages = ( size + PAGE_SIZE - 1 ) / PAGE_SIZE;

	for ( u64 i = 0; i < pages; i++ )
		vmm_map ( table, vaddr, paddr, flags );
}
