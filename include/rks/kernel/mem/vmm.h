#ifndef VMM_H
#define VMM_H

#include <arch_const.h>

#ifndef __ASSEMBLY__

#include <rks/types.h>
#include "pmm.h"

/* Flags for mapping */
typedef enum
{
	VMM_PRESENT = 1 << 0,
	VMM_RW = 1 << 1,
	VMM_USER = 1 << 2,
	VMM_NOCACHE = 1 << 3,
} vmm_map_flags_t;

typedef addr_t vaddr_t;

void vmm_init ( void );
bool vmm_map ( vaddr_t vaddr, paddr_t paddr, vmm_map_flags_t flags );
bool vmm_map_range ( vaddr_t vaddr, paddr_t paddr,
                     size_t size, vmm_map_flags_t flags );
bool vmm_unmap ( vaddr_t vaddr );

/* Debug/testing */
void vmm_dump_tables ( vaddr_t vaddr );

#endif /* __ASSEMBLY__ */
#endif /* VMM_H */
