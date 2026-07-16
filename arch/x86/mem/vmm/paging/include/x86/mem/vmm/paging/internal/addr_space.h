#ifndef _X86_PAGING_ADDR_SPACE_H
#ifndef _X86_PAGING_ADDR_SPACE_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/vmm/types.h>

/* Address space struct */
struct addr_space
{
	/* atomic_size_t refcount */
	/* spinlock_t lock */
	/* struct vm_region_tree regions */
	paddr_t pml4_phys;
};

/* Address space paging operations */
ADDR_SPACE* _paging_create_space ( void );
kerr_t _paging_destroy_space ( ADDR_SPACE * space );
kerr_t _paging_switch_to_space ( ADDR_SPACE * space );

#endif /* _X86_PAGING_ADDR_SPACE_H */
