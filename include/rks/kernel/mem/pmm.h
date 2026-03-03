#ifndef PMM_H
#define PMM_H

#include <rks/types.h>

#define FRAME_SIZE 4096

void pmm_init (void* mbi);

/* PMM context struct */
struct pmm_t
{
	__u64 total_frames;
	__u64 used_frames;
	__u64 bitmap_size;
	__u8* bitmap;		/* Array made out of bits */
};

#endif /* PMM_H */
