#ifndef PMM_INTERNAL_H
#define PMM_INTERNAL_H

#include <kernel/mem/pmm/types.h>
#include <pmm/backend.h>

/* PMM context struct */
struct pmm
{
	struct
	{
		u32 total;
		u32 used;
	} frames;

	struct
	{
		size_t total;
	} memory;

	/* PMM operations */
	struct pmm_ops* ops;

	/* Is it ready to be used? */
	bool ready;
};

/* PMM context struct pointer */
extern struct pmm* const g_pmm;

#endif /* PMM_INTERNAL_H */
