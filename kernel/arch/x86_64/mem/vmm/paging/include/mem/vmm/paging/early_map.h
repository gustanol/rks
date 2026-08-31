#ifndef _X86_PAGING_EARLY_MAP_H
#define _X86_PAGING_EARLY_MAP_H

#include <kernel/mem/pmm/types.h>
#include <kernel/mem/vmm/types.h>

void _early_map_range ( paddr_t * pml4, vaddr_t virt_addr, paddr_t phys_addr,
    size_t size, u32 flags );

#endif /* _X86_PAGING_EARLY_MAP_H */
