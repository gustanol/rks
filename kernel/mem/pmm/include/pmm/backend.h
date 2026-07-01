#ifndef _KERNEL_SUBSYS_PMM_BACKEND_H
#define _KERNEL_SUBSYS_PMM_BACKEND_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/pmm/types.h>
#include <rks/kernel/subsys/registry.h>

#define SUBSYS_TAG_PMM 0x504D4D

/* PMM backend registration macro */
#define REGISTER_PMM_BACKEND(NAME, INIT_FN, OPS) \
        REGISTER_SUBSYSTEM_BACKEND( ( SUBSYSTEM_TAG_PMM ), ( NAME ), ( INIT_FN ), ( OPS ) )

/* PMM backend operations */
struct pmm_ops
{
	paddr_t ( *alloc_frame ) ( u32 );
	kerr_t ( *free_frame ) ( paddr_t, u32 );
	size_t ( *free_region ) ( paddr_t, size_t, u32 );
	size_t ( *fill_region ) ( paddr_t, size_t, u32 );
};

/* Context for init backend functions */
struct pmm_init_backend_ctx
{
	size_t mem_total;
	u32 frames_total;
};

#endif /* _KERNEL_SUBSYS_PMM_BACKEND_H */
