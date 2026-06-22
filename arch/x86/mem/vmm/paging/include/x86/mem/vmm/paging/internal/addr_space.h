#ifndef _X86_PAGING_ADDR_SPACE_H
#ifndef _X86_PAGING_ADDR_SPACE_H

#include <rks/kernel/mem/vmm/types.h>

/* Address space struct */
struct addr_space
{
	paddr_t pml4_phys;
};

/* Address space paging operations */
ADDR_SPACE* _paging_create_space ( void );
void _paging_destroy_space ( ADDR_SPACE * space );
void _paging_switch_to_space ( ADDR_SPACE * space );

#endif /* _X86_PAGING_ADDR_SPACE_H */
