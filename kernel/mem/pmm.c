#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/boot/multiboot2.h>
#include <rks/kernel/klog/print.h>
#include <rks/lib/bitwise.h>

static struct pmm_t pmm;
extern char kernel_start[];
extern char kernel_end[];

/* Prototypes */
static ul64 parse_mmap (struct multiboot2_tag_mmap* mmap);
static void mark_region_free (ul64 addr, __u64 len);
static void mark_region_used (ul64 addr, __u64 len);

void
pmm_init (void* mbi)
{
	/* Memory mapping tag provided by the Multiboot2 info struct */
	struct multiboot2_tag_mmap* mmap =
		find_multiboot2_tag (mbi, MULTIBOOT2_TAG_TYPE_MMAP);

	if (mmap == NULL)
		return;		/* Treat error */

	/* Define the base address of bitmap as being the end of kernel */
	pmm.bitmap = (char *) kernel_end;

	/*
	 * Parse the mmap
	 * Fill the bitmap
	 * Return the total memory
	 */
	ul64 total_memory = parse_mmap (mmap);

	/* How many frames does the physical memory has */
	pmm.total_frames = total_memory / FRAME_SIZE;
	pmm.bitmap_size = (pmm.total_frames + 7) / 8;	/* Divide by 8 to get the size in bytes */

	/* Mark regions we're using */
	mark_region_used ((ul64) kernel_start,
			  (ul64) (kernel_end - kernel_start));
	mark_region_used ((ul64) pmm.bitmap, pmm.bitmap_size);

	kprintf ("pmm: Total memory = %llu MiB\n",
		 total_memory / 1024 / 1024);
	kprintf ("pmm: Total frames = %lu\n", pmm.total_frames);
}

/*
 * Memory map parser
 *
 * -> ul64: The total amound of available addressable memory
 * @mmap: Memory map struct
 */
ul64
parse_mmap (struct multiboot2_tag_mmap* mmap)
{
	/*
	 * Calculate the amount of available mmap entries
	 *
	 * We must consider only the entries to calculate the size itself
	 * The 16 comes from the other fields on mmap tag
	 */
	u32 entry_count = (mmap->size - 16) / mmap->entry_size;
	ul64 total_memory = 0;

	/* Iterate for each mmap entry */
	for (u32 i = 0; i < entry_count; i++)
	{
		/*
		 * Get the current entry by summing the current one offset to `entries` base
		 */
		struct multiboot2_mmap_entry* entry =
			(struct multiboot2_mmap_entry *) ((ul64)
							  mmap->entries +
							  (i *
							   mmap->entry_size));

		/* For each available region, mark it as free and sum it to the total memory */
		if (entry->type == MULTIBOOT2_MEMORY_AVAILABLE)
		{
			mark_region_free (entry->addr, entry->len);
			total_memory += entry->len;
		}
	}

	/* Return the total memory size */
	return total_memory;
}

/*
 * Find the first allocable frame and return its physical address
 *
 * -> u64: Frame address
 */
u64
pmm_alloc_frame (void)
{
	int bit = FIND_FIRST_ZERO_BIT (pmm.bitmap, pmm.bitmap_size);
	if (bit == -1)
		return 0;	/* No available memory */

	BITMAP_SET_BIT (pmm.bitmap, bit);
	pmm.used_frames++;

	/* Each bit of bitmap represents a physical frame */
	return bit * FRAME_SIZE;
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
}
