#ifndef PMM_DISPATCH_H
#define PMM_DISPATCH_H

#include <pmm/internal.h>

/* Dispatch function prototypes
 ================================================================ */
paddr_t _pmm_dispatch_alloc_frame ( void );
u32 _pmm_dispatch_free_frame ( paddr_t phys_addr );
size_t _pmm_dispatch_free_region ( paddr_t phys_addr, size_t size );
size_t _pmm_dispatch_fill_region ( paddr_t phys_addr, size_t size );
size_t _pmm_dispatch_get_total_memory ( void );

/* ============================================================== */

#endif /* PMM_DISPATCH_H */
