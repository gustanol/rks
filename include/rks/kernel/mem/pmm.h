#ifndef _KERNEL_PMM_H
#define _KERNEL_PMM_H

#include <rks/kernel/err/kerr.h>
#include "types.h"

paddr_t pmm_alloc_frame ( void );
paddr_t pmm_alloc_frames ( size_t frames_needed );
kerr_t pmm_free_frame ( paddr_t phys_addr );

u32 pmm_free_region ( paddr_t phys_addr, size_t size );
u32 pmm_fill_region ( paddr_t phys_addr, size_t size );

size_t pmm_get_total_memory ( void );

#endif /* _KERNEL_PMM_H */
