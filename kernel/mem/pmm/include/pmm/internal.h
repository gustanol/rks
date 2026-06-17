#ifndef PMM_INTERNAL_H
#define PMM_INTERNAL_H

#include <rks/kernel/mem/pmm.h>
#include <pmm/backend.h>

/* PMM context struct */
struct pmm
{
	struct
	{
		u64 total;
		u64 used;
	} frames;

	struct
	{
		size_t total;
	} memory;

	/* PMM operations */
	struct pmm_ops* ops;
};

/* PMM context struct pointer */
extern struct pmm* g_pmm;

#endif /* PMM_INTERNAL_H */
