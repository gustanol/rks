#ifndef VMM_H
#define VMM_H

#include <rks/types.h>
#include "pmm.h"

#define PAGE_SIZE 4096

/* Virtual bases */
#define KERNEL_VIRTUAL_BASE 0xFFFFFFFF80000000
#define KERNEL_PHYSMAP_BASE 0xFFFFFFFFC0000000

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

#endif /* VMM_H */
