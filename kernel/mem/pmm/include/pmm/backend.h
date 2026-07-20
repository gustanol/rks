#ifndef _KERNEL_SUBSYS_PMM_BACKEND_H
#define _KERNEL_SUBSYS_PMM_BACKEND_H

#include <rks/kernel/container_of.h>
#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/pmm/types.h>
#include <rks/kernel/subsys/registry.h>

#define SUBSYS_TAG_PMM 0x504D4D

/* PMM backend registration macro */
#define REGISTER_PMM_BACKEND(BACKEND) \
        ({ \
                const struct pmm_backend* __tmp = &( BACKEND ); \
                REGISTER_SUBSYS_BACKEND ( &( __tmp )->base ); \
        })

/* Signature macros */
#define PMM_BACKEND_INIT(name) \
        kerr_t name ( const struct pmm_init_backend_ctx* ctx )

#define PMM_BACKEND_ALLOC_FRAME(name) \
        paddr_t name ( u32 total_frames )

#define PMM_BACKEND_FREE_FRAME(name) \
        kerr_t name ( paddr_t phys_addr, u32 total_frames )

#define PMM_BACKEND_ALLOC_REGION(name) \
        paddr_t name ( size_t frames_needed, u32 total_frames )

#define PMM_BACKEND_FILL_REGION(name) \
        kerr_t name ( paddr_t phys_addr, size_t size, u32 total_frames )

#define PMM_BACKEND_FREE_REGION(name) \
        kerr_t name ( paddr_t phys_addr, size_t size, u32 total_frames )

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
	u32 frames_total;
};

/* PMM backend structure context */
struct pmm_backend
{

	/* Subsystem base pointer */
	const struct subsys_backend base;

	/* Init function */
	  PMM_BACKEND_INIT ( ( *init ) );

	/* Operations */
	const struct pmm_ops const* ops;
};

/* Subsys base -> PMM backend */
static inline struct pmm_backend*
pmm_backend_from_base ( struct subsys_backend* base )
{
	return container_of ( base, struct pmm_backend, base );
}

#endif /* _KERNEL_SUBSYS_PMM_BACKEND_H */
