#include <boot/loader_parser.h>
#include <boot/early_alloc.h>
#include <rks/lib/boot/multiboot2.h>

static void parse_mmap (struct multiboot2_tag_mmap* tag, mmap_tag_t * mmap);

/*
 * Parse the Multiboot2 info struct to populate generic boot layer
 *
 * -> void
 * @info_ptr: 64-bit address of the MB2 info struct
 * @boot_info: Pointer to the generic boot info struct
 */
void
parse_mbi2 (u64 info_ptr, boot_info_t* boot_info)
{
	/* Allocate memory for the MMAP tag */
	boot_info->mmap = (mmap_tag_t *) early_alloc (sizeof (mmap_tag_t));

	/* Get the MB2 MMAP tag from the info pointer */
	struct multiboot2_tag_mmap* mb2_mmap =
		find_multiboot2_tag ((void *) info_ptr,
				     MULTIBOOT2_TAG_TYPE_MMAP);
	parse_mmap (mb2_mmap, boot_info->mmap);
}

/*
 * Parse every Multiboot2 MMAP entry to populate the generic boot layer
 *
 * -> void
 * @tag: Multiboot2 MMAP tag
 * @mmap: Pointer to the generic boot MMAP tag
 *   - Not exclusive to MB2
 */
void
parse_mmap (struct multiboot2_tag_mmap* tag, mmap_tag_t* mmap)
{
	/*
	 * Get the amount of entries Multiboot2 provided
	 *
	 * That `-16` refer to some values we don't care
	 */
	u32 entry_count = (tag->size - 16) / tag->entry_size;
	mmap->entry_count = entry_count;

	/* Allocate memory according on how many entries we have */
	mmap->entries =
		(mmap_entry_t *) early_alloc (sizeof (mmap_entry_t) *
					      entry_count);

	/* Iterate for each MMAP entry */
	for (u32 i = 0; i < entry_count; i++)
	{
		/*
		 * Each MB2 entry can be calculate the following way:
		 * - `tag->entries`: The base pointer
		 * - `i * tag->entry_size`: The offset to entry `i`
		 */
		struct multiboot2_mmap_entry* entry =
			(struct multiboot2_mmap_entry *) ((ul64) tag->entries
							  +
							  (i *
							   tag->entry_size));

		/* Fill out our generic entries with this data */
		mmap->entries[i].addr = entry->addr;
		mmap->entries[i].len = entry->len;
		mmap->entries[i].type = (mem_type_t) entry->type;
	}
}
