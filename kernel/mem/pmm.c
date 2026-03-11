#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/klog/print.h>
#include <rks/lib/bitwise.h>

static struct pmm_t pmm;
extern char kernel_start[];
extern char kernel_end[];

/* Prototypes */
static void mark_region_free (ul64 addr, u64 len);
static void mark_region_used (ul64 addr, u64 len);

void
pmm_init (mmap_tag_t* mmap)
{
	/* We can't continue if it's NULL */
	if (mmap == NULL)
		return;		/* Treat error */

	/* Define the base address of bitmap as being the end of kernel */
	pmm.bitmap = (char *) kernel_end;

	/* Total memory size in bytes */
	ul64 total_memory = 0;

	/* Iterate for each entry, incrementing the total_memory by the current entry length */
	for (u32 i = 0; i < mmap->entry_count; i++)
	{
		/* Only get available memory */
		if (mmap->entries[i].type == MEM_AVAILABLE)
		{
			/* Increment the total memory */
			total_memory += mmap->entries[i].len;

			/* Mark this region as free */
			mark_region_free (mmap->entries[i].addr,
					  mmap->entries[i].len);
		}
	}

	/* How many frames does the physical memory has */
	pmm.total_frames = total_memory / FRAME_SIZE;

	/* Divide by 8 to get the size in bytes */
	pmm.bitmap_size = (pmm.total_frames + 7) / 8;
	pmm.used_frames = 0;

	/* Mark regions we're using */
	mark_region_used ((ul64) kernel_start,
			  (ul64) (kernel_end - kernel_start));
	mark_region_used ((ul64) pmm.bitmap, pmm.bitmap_size);

	kprintf ("PMM: Available memory = %llu MiB\n",
		 total_memory / 1024 / 1024);
	kprintf ("PMM: Total frames = %lu\n", pmm.total_frames);
	kprintf ("PMM: Used frames = %lu\n", pmm.used_frames);
}

/*
 * Find the first allocable frame and return its physical address
 *
 * -> u64: Frame address
 */
u64
pmm_alloc_frames (u64 frames_needed)
{
	long bit;		/* The bit can return -1 in errors */
	u64 first_bit;		/* For multiple allocations, we must return the address for the first frame */

	/* Repeat the operation as the value of `frames_needed` */
	for (u64 i = 0; i < frames_needed; i++, pmm.used_frames++)
	{
		/* Find the first available frame by looking at the bitmap */
		bit = FIND_FIRST_ZERO_BIT (pmm.bitmap, pmm.bitmap_size);

		/*
		 * In errors, bit can be -1
		 */
		if (bit == -1)
		{
			/* Clean already set bits soon */
			return 0;	/* No available memory */
		}

		BITMAP_SET_BIT (pmm.bitmap, bit);

		/* Only get it at the first iteration */
		if (!i)
			first_bit = bit;
	}

	/* Each bit of bitmap represents a physical frame */
	return first_bit * FRAME_SIZE;
}

/*
 * Free an allocated frame receiving its physical memory
 *
 * -> void
 * @phys_addr: Frame physical address
 */
void
pmm_free_frame (u64 phys_addr)
{
	/* Physical frame */
	u64 frame = phys_addr / FRAME_SIZE;

	/* Zero this index in bitmap */
	BITMAP_CLEAR_BIT (pmm.bitmap, frame);
	pmm.used_frames--;
}

/*
 * Mark a memory region as free to use
 *
 * -> void
 * @addr: Start address
 * @len: Limit to be marked
 */
void
mark_region_free (ul64 addr, u64 len)
{
	/* To select a bit from the bitmap, we need to get the physical frame first */
	u64 start = addr / FRAME_SIZE;

	/* At the same principle, we need to get how many frames the size has */
	u64 count = len / FRAME_SIZE;

	/* Clear all bits in this interval */
	for (u64 i = 0; i < count; i++)
		BITMAP_CLEAR_BIT (pmm.bitmap, start + i);

	pmm.used_frames -= count;
}

/*
 * Mark a memory region as used
 *
 * -> void
 * @addr: Start address
 * @len: Limit of marking
 */
void
mark_region_used (ul64 addr, u64 len)
{
	u64 start = addr / FRAME_SIZE;
	u64 count = len / FRAME_SIZE;

	/* Set all bits in this interval */
	for (u64 i = 0; i < count; i++)
		BITMAP_SET_BIT (pmm.bitmap, start + i);

	pmm.used_frames += count;
}
