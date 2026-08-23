#ifndef _X86_PAGING_MAP_H
#define _X86_PAGING_MAP_H

#include <kernel/kerr/kerr.h>
#include <kernel/mem/pmm/types.h>
#include <kernel/mem/vmm/types.h>

/* Mapping paging operations */
kerr_t _paging_map ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
    vaddr_t hhdm_offset, u32 flags );
kerr_t _paging_unmap ( ADDR_SPACE * space, vaddr_t virt_addr, vaddr_t hhdm_offset );
kerr_t _paging_unmap_identity ( ADDR_SPACE * kernel_address_space );
kerr_t _paging_remap ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
    vaddr_t hhdm_offset, u32 flags );

/* Translation operations */
paddr_t _paging_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr, vaddr_t hhdm_offset );
vaddr_t _paging_phys_to_virt ( paddr_t phys_addr, vaddr_t hhdm_offset );

#endif /* _X86_PAGING_MAP_H */
