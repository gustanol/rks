#ifndef _KERNEL_VMM_MAP_H
#define _KERNEL_VMM_MAP_H

#include "types.h"
#include <rks/kernel/mem/pmm/types.h>

/* Generic VMM flags */
enum vmm_flags
{
	VMM_READ = 0 << 1;
	VMM_WRITE = 1 << 1;
	VMM_EXEC = 1 << 2;
	VMM_USER = 1 << 3;
};

/* Mapping operations */
int vmm_map ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
              u32 flags );
int vmm_unmap ( ADDR_SPACE * space, vaddr_t virt_addr );
int vmm_remap ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
                u32 flags );

/* Convert operations */
paddr_t vmm_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr );
vaddr_t vmm_phys_to_virt ( paddr_t phys_addr );

#endif /* _KERNEL_VMM_MAP_H */
