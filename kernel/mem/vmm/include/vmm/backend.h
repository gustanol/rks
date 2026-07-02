#ifndef _KERNEL_VMM_BACKEND_H
#define _KERNEL_VMM_BACKEND_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/vmm/types.h>
#include <rks/kernel/subsys/registry.h>

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
	  kerr_t ( *destroy_space ) ( ADDR_SPACE * );

	/* Address space switching */
	  kerr_t ( *switch_to_space ) ( ADDR_SPACE * );

	/* Virtual address mapping within an address space */
	  kerr_t ( *map ) ( ADDR_SPACE *, vaddr_t, paddr_t, vaddr_t,
	                    u32 flags );

	/* Virtual address unmapping within an address space */
	  kerr_t ( *unmap ) ( ADDR_SPACE *, vaddr_t, vaddr_t );

	/* Remapping operation */
	  kerr_t ( *remap ) ( ADDR_SPACE *, vaddr_t, vaddr_t, u32 flags );

	/**
         * Convertions of virtual to physical address
         **/
	  paddr_t ( *virt_to_phys ) ( ADDR_SPACE *, vaddr_t, vaddr_t );
	  vaddr_t ( *phys_to_virt ) ( paddr_t, vaddr_t );
};

#endif /* _KERNEL_VMM_BACKEND_H */
