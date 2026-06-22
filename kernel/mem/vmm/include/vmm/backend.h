#ifndef VMM_BACKEND_H
#define VMM_BACKEND_H

#include <rks/kernel/subsys/registry.h>
#include <rks/kernel/mem/vmm.h>

#define SUBSYS_TAG_VMM 0x564D4D

/* VMM backend registration macro */
#define REGISTER_VMM_BACKEND(NAME, INIT_FN, OPS) \
        REGISTER_SUBSYSTEM_BACKEND( ( SUBSYSTEM_TAG_VMM ), ( NAME ), ( INIT_FN ), ( OPS ) )

/**
 * VMM private operations
 *
 * It's gonna be defined by each architecture-dependent backend
 **/
struct vmm_ops
{
	/* Early higher half direct mapping constructor */
	void ( *early_build_hhdm ) ( ADDR_SPACE * );

	/**
         * Early identity unmapping
         * It can be shared between architectures
         **/
	void ( *early_identity_unmap ) ( ADDR_SPACE * );

	/* Address space creation */
	ADDR_SPACE* ( *create_space ) ( void );

	/* Address space destruction */
	void ( *destroy_space ) ( ADDR_SPACE * );

	/* Address space switching */
	void ( *switch_to_space ) ( ADDR_SPACE * );

	/* Virtual address mapping within an address space */
	int ( *map ) ( ADDR_SPACE *, vaddr_t, paddr_t, vaddr_t, u32 flags );

	/* Virtual address unmapping within an address space */
	int ( *unmap ) ( ADDR_SPACE *, vaddr_t, vaddr_t );

	/* Remapping operation */
	int ( *remap ) ( ADDR_SPACE *, vaddr_t, vaddr_t, u32 flags );

	/* Convertion of a virtual address to a physical one within an address space */
	  paddr_t ( *virt_to_phys ) ( ADDR_SPACE *, vaddr_t, vaddr_t );

	  vaddr_t ( *phys_to_virt ) ( paddr_t, vaddr_t );
};

#endif /* VMM_BACKEND_H */
