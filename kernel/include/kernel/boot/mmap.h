#ifndef _KERNEL_BOOT_MMAP_H
#define _KERNEL_BOOT_MMAP_H

#include <stddef.h>
#include <stdint.h>

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
	uint64_t addr;

	/* Entry length */
	size_t len;

	/* Memory type */
	uint32_t type;
} __attribute__ ( ( packed ) );

/* Generic parsed MMAP tag */
struct mmap_tag
{

	/* Amount of entries */
	uint32_t entry_count;

	/* Pointer to them */
	struct mmap_entry* entries;
} __attribute__ ( ( packed ) );

#endif /* _KERNEL_BOOT_MMAP_H */
