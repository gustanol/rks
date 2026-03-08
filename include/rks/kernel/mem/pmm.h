#ifndef PMM_H
#define PMM_H

#include <rks/types.h>

#define FRAME_SIZE 4096

void pmm_init (void* mbi);
u64 pmm_alloc_frames (u64 frames_needed);
void pmm_free_frame (u64 phys_addr);

/* PMM context struct */
struct pmm_t
{
	u64 total_frames;
	u64 used_frames;
	u64 bitmap_size;
	char* bitmap;		/* Array made out of bits */
};

#endif /* PMM_H */
