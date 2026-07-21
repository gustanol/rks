#include <rks/kernel/boot/mmap.h>
#include <rks/lib/boot/multiboot2.h>
#include <x86/mem/early_alloc.h>

/**
 * Parses all Multiboot2-provided MMAP entries to populates the boot info struct
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct multiboot2_tag_mmap* tag`: Multiboot2-specific MMAP tag
 *   2. `struct mmap_tag* g_mmap`: Global kernel MMAP state
 * Callers:
 *   - `_parse_mbi2`
 **/
void
__parse_mmap ( struct multiboot2_tag_mmap* tag, struct mmap_tag* g_mmap )
{

	/**
	 * Amount of MMAP entries Multiboot2 provided
	 *
	 * Ignores the header to count entries
	 **/
	u32 entry_count = ( tag->size - 16 ) / tag->entry_size;

	/* Fills the tag */
	g_mmap->entry_count = entry_count;

	/* Allocates memory according on how many entries were provided */
	g_mmap->entries =
	        ( struct mmap_entry * )
	        _early_alloc ( sizeof ( struct mmap_entry ) * entry_count, 0x10,
	                       FILL_ZEROS );

	/* Iterates for each MMAP entry */
	for ( u32 entry_idx = 0; entry_idx < entry_count; entry_idx++ )
	{
		/*
		 * Each MB2 entry can be calculated the following way:
		 * - `tag->entries`: The base pointer
		 * - `entry_idx * tag->entry_size`: The offset to entry `entry_idx`
		 */
		struct multiboot2_mmap_entry* entry =
		        ( struct multiboot2_mmap_entry * ) ( ( paddr_t )
		                                             tag->entries +
		                                             ( entry_idx *
		                                               tag->entry_size ) );

		/* Fills the address */
		g_mmap->entries[entry_idx].addr = entry->addr;

		/* Entry length covering */
		g_mmap->entries[entry_idx].len = entry->len;

		/* Entry memory type */
		g_mmap->entries[entry_idx].type = ( enum mem_type ) entry->type;
	}
}
