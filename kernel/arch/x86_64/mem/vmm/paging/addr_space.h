#ifndef _X86_64_PAGING_ADDR_SPACE_H
#define _X86_64_PAGING_ADDR_SPACE_H

#include <kernel/kerr/kerr.h>
#include <kernel/mem/pmm/types.h>
#include <stddef.h>
#include <stdint.h>
#include <vmm/backend.h>

/* Address space struct */
struct addr_space
{
	/* atomic_size_t refcount */
	/* spinlock_t lock */
	/* struct vm_region_tree regions */
	paddr_t pml4_phys;
};

/* Address space paging operations */
VMM_BACKEND_CREATE_SPACE ( _paging_create_space );
VMM_BACKEND_DESTROY_SPACE ( _paging_destroy_space );
VMM_BACKEND_SWITCH_TO_SPACE ( _paging_switch_to_space );

#endif /* _X86_64_PAGING_ADDR_SPACE_H */
