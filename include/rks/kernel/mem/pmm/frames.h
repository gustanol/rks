#ifndef _KERNEL_PMM_FRAMES_H
#define _KERNEL_PMM_FRAMES_H

#include "types.h"
#include <rks/kernel/err/kerr.h>

/* Amount of frames to fit `SIZE` */
#define FRAMES_BY_SIZE(SIZE) \
        ( ( ( SIZE ) + ( FRAME_SIZE ) - 1 ) / ( FRAME_SIZE ) )

/* Size of `n` frames */
#define SIZE_BY_FRAMES(n) \
        ( ( FRAME_SIZE ) * ( n ) )

paddr_t pmm_alloc_frame ( void );
paddr_t pmm_alloc_frames ( size_t frames_needed );
kerr_t pmm_free_frame ( paddr_t phys_addr );

u32 pmm_free_region ( paddr_t phys_addr, size_t size );
u32 pmm_fill_region ( paddr_t phys_addr, size_t size );

#endif /* _KERNEL_PMM_FRAMES_H */
