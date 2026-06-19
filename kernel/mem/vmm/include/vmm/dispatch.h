#ifndef VMM_DISPATCH_H
#define VMM_DISPATCH_H

#include <vmm/internal.h>

/* Dispatch function prototypes
 * ============================================================================== */
ADDR_SPACE* _vmm_dispatch_create_space ( void );
void _vmm_dispatch_destroy_space ( ADDR_SPACE * space );
void _vmm_dispatch_switch_to_space ( ADDR_SPACE * space );

int _vmm_dispatch_map ( ADDR_SPACE * space, vaddr_t virt_addr,
                        paddr_t phys_addr, u32 flags );
int _vmm_dispatch_unmap ( ADDR_SPACE * space, vaddr_t virt_addr );
int _vmm_dispatch_remap ( ADDR_SPACE * space, vaddr_t virt_addr,
                          paddr_t phys_addr, u32 flags );

paddr_t _vmm_dispatch_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr );
vaddr_t _vmm_dispatch_phys_to_virt ( paddr_t phys_addr );

void _vmm_dispatch_flush_tlb ( ADDR_SPACE * space, vaddr_t virt_addr,
                               size_t size );

/* ============================================================================== */

#endif /* VMM_DISPATCH_H */
