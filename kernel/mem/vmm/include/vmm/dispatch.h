#ifndef _KERNEL_SUBSYS_VMM_DISPATCH_H
#define _KERNEL_SUBSYS_VMM_DISPATCH_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/pmm/types.h>
#include <rks/kernel/mem/vmm/types.h>

/* Dispatch function prototypes
 * ============================ */
ADDR_SPACE* _vmm_dispatch_create_space ( void );
kerr_t _vmm_dispatch_destroy_space ( ADDR_SPACE * space );
kerr_t _vmm_dispatch_switch_to_space ( ADDR_SPACE * space );

kerr_t _vmm_dispatch_map ( ADDR_SPACE * space, vaddr_t virt_addr,
                           paddr_t phys_addr, u32 flags );
kerr_t _vmm_dispatch_unmap ( ADDR_SPACE * space, vaddr_t virt_addr );
kerr_t _vmm_dispatch_remap ( ADDR_SPACE * space, vaddr_t virt_addr,
                             paddr_t phys_addr, u32 flags );

paddr_t _vmm_dispatch_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr );
vaddr_t _vmm_dispatch_phys_to_virt ( paddr_t phys_addr );

#endif /* _KERNEL_SUBSYS_VMM_DISPATCH_H */
