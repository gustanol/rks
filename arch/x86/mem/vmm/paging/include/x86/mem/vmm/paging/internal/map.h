#ifndef _X86_PAGING_MAP_H
#define _X86_PAGING_MAP_H

#include <rks/kernel/mem/pmm/types.h>
#include <rks/kernel/mem/vmm/types.h>

/* Mapping paging operations */
int _paging_map ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
                  vaddr_t hhdm_offset, u32 flags );
int _paging_unmap ( ADDR_SPACE * space, vaddr_t virt_addr,
                    vaddr_t hhdm_offset );
int _paging_remap ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
                    vaddr_t hhdm_offset, u32 flags );

/* Translation operations */
paddr_t _paging_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr,
                               vaddr_t hhdm_offset );
vaddr_t _paging_phys_to_virt ( paddr_t phys_addr, vaddr_t hhdm_offset );

#endif /* _X86_PAGING_MAP_H */
