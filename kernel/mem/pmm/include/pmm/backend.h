#ifndef _KERNEL_SUBSYS_PMM_BACKEND_H
#define _KERNEL_SUBSYS_PMM_BACKEND_H

#include <kernel/container_of.h>
#include <kernel/kerr/kerr.h>
#include <kernel/mem/pmm/types.h>
#include <kernel/subsys/registry.h>

#define SUBSYS_TAG_PMM 0x504D4D

/* PMM backend registration macro */
#define REGISTER_PMM_BACKEND(BACKEND) \
    REGISTER_SUBSYSTEM_BACKEND ( ( BACKEND ).base )

/**
 * PMM backend initialization entry
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `const struct pmm_init_backend_ctx* ctx`: PMM init context 
 * Prototype
 **/
#define PMM_BACKEND_INIT(name) \
        kerr_t name ( const struct pmm_init_backend_ctx* ctx )

/**
 * PMM backend frame allocator
 *
 * Return -> `paddr_t`: Physical address
 * Arguments:
 *   1. `uint32_t total_frames`: Total amount of frames
 * Prototype
 **/
#define PMM_BACKEND_ALLOC_FRAME(name) \
        paddr_t name ( uint32_t total_frames )

/**
 * PMM backend frame freer 
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame base address
 *   2. `uint32_t total_frames`: Total frames
 * Prototype
 **/
#define PMM_BACKEND_FREE_FRAME(name) \
        kerr_t name ( paddr_t phys_addr, uint32_t total_frames )

/**
 * PMM backend region allocator
 *
 * Return -> `paddr_t`: Physical address
 * Arguments:
 *   1. `size_t frames_needed`: Amount of frames to be allocated
 *   2. `uint32_t total_frames`: Total amount of frames
 * Prototype
 **/
#define PMM_BACKEND_ALLOC_REGION(name) \
        paddr_t name ( size_t frames_needed, uint32_t total_frames )

/**
 * PMM backend region filler
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base address
 *   2. `size_t size`: Region size
 *   3. `uint32_t total_frames`: Total frames
 * Prototype
 **/
#define PMM_BACKEND_FILL_REGION(name) \
        kerr_t name ( paddr_t phys_addr, size_t size, uint32_t total_frames )

/**
 * PMM backend region freer
 *
 * Return -> `kerr_t`: Global error code
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base address
 *   2. `size_t size`: Region size
 *   3. `uint32_t total_frames`: Total frames
 * Prototype
 **/
#define PMM_BACKEND_FREE_REGION(name) \
        kerr_t name ( paddr_t phys_addr, size_t size, uint32_t total_frames )

/* PMM backend operations */
struct pmm_ops
{

	/* Allocates a single frame */
	PMM_BACKEND_ALLOC_FRAME ( ( *alloc_frame ) );

	/* Frees a single frame */
	PMM_BACKEND_FREE_FRAME ( ( *free_frame ) );

	/* Allocates a contiguous memory region */
	PMM_BACKEND_ALLOC_REGION ( ( *alloc_region ) );

	/* Fills a contiguous memory region */
	PMM_BACKEND_FILL_REGION ( ( *fill_region ) );

	/* Frees a contiguous memory region */
	PMM_BACKEND_FREE_REGION ( ( *free_region ) );
};

/* Context for init backend functions */
struct pmm_init_backend_ctx
{
	size_t mem_total;
	uint32_t frames_total;
};

/* PMM backend structure context */
struct pmm_backend
{

	/* Subsystem base pointer */
	const struct subsys_backend base;

	/* Init function */
	  PMM_BACKEND_INIT ( ( *init ) );

	/* Operations */
	const struct pmm_ops* ops;
};

/* Subsys base -> PMM backend */
static inline struct pmm_backend*
pmm_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct pmm_backend, base );
}

#endif /* _KERNEL_SUBSYS_PMM_BACKEND_H */
