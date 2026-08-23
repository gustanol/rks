#ifndef _KERNEL_VMM_SUBSYS_BACKEND_H
#define _KERNEL_VMM_SUBSYS_BACKEND_H

#include <kernel/container_of.h>
#include <kernel/kerr/kerr.h>
#include <kernel/mem/vmm/types.h>
#include <kernel/subsys/registry.h>

#define SUBSYS_TAG_VMM 0x564D4D

/* VMM backend registration macro */
#define REGISTER_VMM_BACKEND(BACKEND) \
	({ \
		const struct vmm_backend* __tmp = &( BACKEND ); \
		REGISTER_SUBSYS_BACKEND ( &( __tmp )->base ); \
	})

/**
 * VMM backend initialization function
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE* kernel_addr_space`: The kernel address space pointer
 * Prototype
 **/
#define VMM_BACKEND_INIT(name) \
	kerr_t name ( const ADDR_SPACE* kernel_addr_space )

/**
 * VMM backend HHDM builder
 *
 * Return -> `void`
 * Arguments:
 *   1. `ADDR_SPACE* kernel_addr_space`: The kernel address space pointer
 * Prototype
 **/
#define VMM_BACKEND_BUILD_HHDM(name) \
	void name ( const ADDR_SPACE* kernel_addr_space )

/**
 * VMM backend address space creator
 *
 * Return -> `ADDR_SPACE *`: The created address space
 * Arguments:
 *   1. `void`
 * Prototype
 **/
#define VMM_BACKEND_CREATE_SPACE(name) \
	ADDR_SPACE* name ( void )

/**
 * VMM backend address space destroyer 
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE * space`: Address space to be destroyed
 * Prototype
 **/
#define VMM_BACKEND_DESTROY_SPACE(name) \
	kerr_t name ( ADDR_SPACE* space )

/**
 * VMM backend function used to active the given address space
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE * space`: Address space to be switched to
 * Prototype
 **/
#define VMM_BACKEND_SWITCH_TO_SPACE(name) \
	kerr_t name ( ADDR_SPACE* space )

/**
 * VMM backend mapper
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the page in
 *   2. `vaddr_t virt_addr`: Virtual address
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `vaddr_t hhdm_offset`: Kernel HHDM offset
 *   5. `uint32_t flags`
 * Prototype
 **/
#define VMM_BACKEND_MAP(name) \
	kerr_t name \
	( \
		ADDR_SPACE* space, \
		vaddr_t virt_addr, \
		paddr_t phys_addr, \
		vaddr_t hhdm_offset, \
		uint32_t flags \
	)

/**
 * VMM backend unmapper
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE* space`: Addess space the page table should be unmapped from
 *   2. `vaddr_t virt_addr`: Page virtual address
 *   3. `vaddr_t hhdm_offset`: Kernel HHDM offset
 * Prototype
 **/
#define VMM_BACKEND_UNMAP(name) \
	kerr_t name ( ADDR_SPACE* space, vaddr_t virt_addr, vaddr_t hhdm_offset )

/**
 * VMM backend function used to unmap the identity mapping
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE* kernel_addr_space`
 * Prototype
 **/
#define VMM_BACKEND_UNMAP_IDENTITY(name) \
	kerr_t ( *unmap_identity ) ( ADDR_SPACE* kernel_addr_space )

/**
 * VMM backend remapper
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `ADDR_SPACE* space`: Address space to map the page in
 *   2. `vaddr_t virt_addr`: Virtual address
 *   3. `paddr_t phys_addr`: Physical address
 *   4. `vaddr_t hhdm_offset`: Kernel HHDM offset
 *   5. `uint32_t flags`
 * Prototype
 **/
#define VMM_BACKEND_REMAP(name) \
	kerr_t name \
	( \
		ADDR_SPACE* space, \
		vaddr_t virt_addr, \
		paddr_t phys_addr, \
		vaddr_t hhdm_offset, \
		uint32_t flags \
	)

/**
 * VMM backend function used to coverter a virtual address to a physical one
 *
 * Return -> `paddr_t`: The converted physical address 
 * Arguments:
 *   1. `ADDR_SPACE* space`: Addess space the page table should be unmapped from
 *   2. `vaddr_t virt_addr`: Page virtual address
 *   3. `vaddr_t hhdm_offset`: Kernel HHDM offset
 * Prototype
 **/
#define VMM_BACKEND_VIRT_TO_PHYS(name) \
	paddr_t name ( ADDR_SPACE* space, vaddr_t virt_addr, vaddr_t hhdm_offset )

/**
 * VMM backend function used to coverter a physical address to a virtual one
 *
 * Return -> `vaddr_t`: The converted virtual address 
 * Arguments:
 *   1. `vaddr_t virt_addr`: Page virtual address
 *   2. `vaddr_t hhdm_offset`: Kernel HHDM offset
 * Prototype
 **/
#define VMM_BACKEND_PHYS_TO_VIRT(name) \
	vaddr_t name ( paddr_t phys_addr, vaddr_t hhdm_offset );

/**
 * VMM private operations
 *
 * It's gonna be defined by each architecture-dependent backend
 **/
struct vmm_ops
{

	/* Early higher half direct mapping constructor */
	VMM_BACKEND_BUILD_HHDM ( ( *early_build_hhdm ) );

	/* Address space creation */
	VMM_BACKEND_CREATE_SPACE ( ( *create_space ) );

	/* Address space destruction */
	VMM_BACKEND_DESTROY_SPACE ( ( *destroy_space ) );

	/* Address space switching */
	VMM_BACKEND_SWITCH_TO_SPACE ( ( *switch_to_space ) );

	/* Virtual address mapping within an address space */
	VMM_BACKEND_MAP ( ( *map ) );

	/* Virtual address unmapping within an address space */
	VMM_BACKEND_UNMAP ( ( *unmap ) );

	/* Identity unmapping */
	VMM_BACKEND_UNMAP_IDENTITY ( ( *unmap_identity ) );

	/* Remapping operation */
	VMM_BACKEND_REMAP ( ( *remap ) );

	/* Virtual address -> Physical address */
	VMM_BACKEND_VIRT_TO_PHYS ( ( *virt_to_phys ) );

	/* Physical address -> Virtual address */
	VMM_BACKEND_PHYS_TO_VIRT ( ( *phys_to_virt ) );
};

/* VMM backend structure context */
struct vmm_backend
{

	/* Subsystem backend base pointer */
	const struct subsys_backend base;

	/* Init function pointer */
	  VMM_BACKEND_INIT ( ( *init ) );

	/* Operations */
	const struct vmm_ops* ops;
};

/* Subsystem base -> VMM backend */
static inline struct vmm_backend*
vmm_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct vmm_backend, base );
}

#endif /* _KERNEL_VMM_SUBSYS_BACKEND_H */
