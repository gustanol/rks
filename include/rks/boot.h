#ifndef BOOT_H
#define BOOT_H

#include <rks/types.h>

/* Information about the bootloader */
typedef struct
{
	u32 magic;
	addr_t info_ptr;
} __attribute__ ( ( packed ) ) loader_info_t;

/* Memory types */
typedef enum
{
	MEM_AVAILABLE = 1,
	MEM_RESERVED = 2,
	MEM_ACPI = 3,
} mem_type_t;

typedef struct
{
	addr_t addr;
	size_t len;
	u32 type;
} mmap_entry_t;

typedef struct
{
	u32 entry_count;
	mmap_entry_t* entries;
} mmap_tag_t;

typedef struct __attribute__ ( ( packed ) )
{
	char signature[8];
	ubyte_t checksum;
	char oemid[6];
	ubyte_t revision;
	addr32_t rsdt_address;

	/* ACPI 2.0+ */
	u32 length;
	addr_t xsdt_address;
	ubyte_t extended_checksum;
	ubyte_t reserved[3];
} rsdp_t;

typedef struct
{
	mmap_tag_t* mmap;

	struct
	{
		void* ptr;
		u32 width;
		u32 height;
	} fb;                   /* Framebuffer */

	rsdp_t* rsdp;
	const char* cmdline;
} boot_info_t;

void parse_boot ( loader_info_t * loader_info, boot_info_t * boot_info );

#endif /* BOOT_H */
