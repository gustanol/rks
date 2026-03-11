#ifndef BOOT_H
#define BOOT_H

#include <rks/types.h>

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36D76289

typedef struct
{
	u32 magic;
	u64 info_ptr;
} __attribute__ ((packed)) loader_info_t;

typedef enum
{
	MEM_AVAILABLE = 1,
	MEM_RESERVED = 2,
	MEM_ACPI = 3,
} mem_type_t;

typedef struct
{
	u64 addr;
	u64 len;
	u32 type;
} mmap_entry_t;

typedef struct
{
	u32 entry_count;
	mmap_entry_t* entries;
} mmap_tag_t;

typedef struct
{
	mmap_tag_t* mmap;
	void* fb_ptr;
	u32 fb_width;
	u32 fb_height;
	void* rsdp;
	const char* cmdline;
} boot_info_t;

void parse_boot (loader_info_t * loader_info, boot_info_t * boot_info);

#endif /* BOOT_H */
