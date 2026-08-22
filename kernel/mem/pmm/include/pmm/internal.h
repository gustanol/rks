#ifndef _KERNEL_PMM_INTERNAL_H
#define _KERNEL_PMM_INTERNAL_H

#include <kernel/mem/pmm/types.h>
#include <pmm/backend.h>

/* PMM context struct */
struct pmm
{
	struct
	{
		uint32_t total;
		uint32_t used;
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

#endif /* _KERNEL_PMM_INTERNAL_H */
