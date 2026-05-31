#ifndef PMM_BACKEND_H
#define PMM_BACKEND_H

#include <rks/kernel/subsys/registry.h>
#include <rks/kernel/mem/pmm.h>

/* PMM backend registration macro */
#define REGISTER_PMM_BACKEND(NAME, INIT_FN, OPS) \
        REGISTER_SUBSYSTEM_BACKEND( ( SUBSYSTEM_TAG_PMM ), ( NAME ), ( INIT_FN ), ( OPS ) )

/* PMM backend operations */
struct pmm_ops
{
	paddr_t ( *alloc_frame ) ( void );
	u32 ( *free_frame ) ( paddr_t );
	size_t ( *free_region ) ( paddr_t, size_t );
	size_t ( *fill_region ) ( paddr_t, size_t );
};

/* Context for init backend functions */
struct pmm_init_backend_ctx
{
	size_t mem_total;
	u64 frames_total;
};

#endif /* PMM_BACKEND_H */
