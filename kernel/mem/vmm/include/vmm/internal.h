#ifndef VMM_INTERNAL_H
#define VMM_INTERNAL_H

#include "backend.h"
#include <rks/kernel/mem/vmm.h>

/* VMM context struct */
struct vmm
{
	struct
	{
		/* Kernel virtual base */
		vaddr_t virt_base;

		/* Kernel virtual end */
		vaddr_t virt_end;

		/* Kernel address space */
		ADDR_SPACE* addr_space;
	} kernel;

	/* Higher-half direct map offset */
	vaddr_t hhdm_offset;

	/* Is VMM ready? */
	bool ready;

	/* VMM private operations */
	struct vmm_ops* ops;
};

/* VMM global state pointer */
extern struct vmm* g_vmm;

#endif /* VMM_INTERNAL_H */
