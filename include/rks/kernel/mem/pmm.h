#ifndef PMM_H
#define PMM_H

#include <rks/types.h>
#include <rks/boot.h>

#define FRAME_SIZE PAGE_SIZE

typedef addr_t paddr_t;

void pmm_init ( struct mmap_tag* mmap );

paddr_t pmm_alloc_frame ( void );
paddr_t pmm_alloc_frames ( size_t frames_needed );
u32 pmm_free_frame ( paddr_t phys_addr );
size_t pmm_free_region ( paddr_t phys_addr, size_t size );
size_t pmm_fill_region ( paddr_t phys_addr, size_t size );
size_t pmm_get_total_memory ( void );

#endif /* PMM_H */
