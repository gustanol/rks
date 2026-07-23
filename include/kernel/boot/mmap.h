#ifndef KERNEL_BOOT_MMAP_H
#define KERNEL_BOOT_MMAP_H

#include <rks/kernel/types.h>

/* Memory types */
enum mem_type
{
	MEM_AVAILABLE = 1,
	MEM_RESERVED = 2,
	MEM_ACPI = 3,
};

/* MMAP entry structure */
struct mmap_entry
{
	/* Base physical address */
	addr_t addr;

	/* Entry length */
	size_t len;

	/* Memory type */
	u32 type;
} __attribute__ ( ( packed ) );

/* Generic parsed MMAP tag */
struct mmap_tag
{
	/* Amount of entries */
	u32 entry_count;

	/* Pointer to them */
	struct mmap_entry* entries;
} __attribute__ ( ( packed ) );

#endif /* KERNEL_BOOT_MMAP_H */
