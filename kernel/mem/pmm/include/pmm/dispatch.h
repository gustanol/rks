#ifndef _KERNEL_SUBSYS_PMM_DISPATCH_H
#define _KERNEL_SUBSYS_PMM_DISPATCH_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/pmm/types.h>

/* Dispatch function prototypes
 * ============================ */
kerr_t _pmm_dispatch_alloc_frame ( paddr_t ** page );
kerr_t _pmm_dispatch_free_frame ( paddr_t phys_addr );

kerr_t _pmm_dispatch_free_region ( paddr_t phys_addr, size_t** size );
kerr_t _pmm_dispatch_fill_region ( paddr_t phys_addr, size_t** size );

size_t _pmm_dispatch_get_total_memory ( void );

#endif /* _KERNEL_SUBSYS_PMM_DISPATCH_H */
