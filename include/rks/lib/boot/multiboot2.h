#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <rks/types.h>

#define MULTIBOOT2_MEMORY_AVAILABLE 1
#define MULTIBOOT2_MEMORY_RESERVED 2
#define MULTIBOOT2_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT2_MEMORY_NVS 4
#define MULTIBOOT2_MEMORY_BADRAM 5

#define MULTIBOOT2_TAG_TYPE_END 0
#define MULTIBOOT2_TAG_TYPE_CMDLINE 1
#define MULTIBOOT2_TAG_TYPE_BOOT_LOADER_NAME 2
#define MULTIBOOT2_TAG_TYPE_MODULE 3
#define MULTIBOOT2_TAG_TYPE_BASIC_MEMINFO 4
#define MULTIBOOT2_TAG_TYPE_BOOTDEV 5
#define MULTIBOOT2_TAG_TYPE_MMAP 6
#define MULTIBOOT2_TAG_TYPE_VBE 7
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8
#define MULTIBOOT2_TAG_TYPE_ELF_SECTIONS 9
#define MULTIBOOT2_TAG_TYPE_APM 10
#define MULTIBOOT2_TAG_TYPE_EFI32 11
#define MULTIBOOT2_TAG_TYPE_EFI64 12
#define MULTIBOOT2_TAG_TYPE_SMBIOS 13
#define MULTIBOOT2_TAG_TYPE_ACPI_OLD 14
#define MULTIBOOT2_TAG_TYPE_ACPI_NEW 15
#define MULTIBOOT2_TAG_TYPE_NETWORK 16
#define MULTIBOOT2_TAG_TYPE_EFI_MMAP 17
#define MULTIBOOT2_TAG_TYPE_EFI_BS 18
#define MULTIBOOT2_TAG_TYPE_EFI32_IH 19
#define MULTIBOOT2_TAG_TYPE_EFI64_IH 20
#define MULTIBOOT2_TAG_TYPE_LOAD_BASE_ADDR 21

typedef struct
{
	u32 type;
	u32 size;
} multiboot2_tag_t;

typedef struct
{
	u32 type;
	u32 size;
	char string[];
} multiboot2_tag_cmdline_t;

typedef struct
{
	u64 addr;
	u64 len;
	u32 type;
	pad_t zero[2];
} multiboot2_mmap_entry_t;

typedef struct
{
	u32 type;
	u32 size;
	u32 entry_size;
	u32 entry_version;
	multiboot2_mmap_entry_t entries[];
} multiboot2_tag_mmap_t;

typedef struct
{
	u32 type;
	u32 size;
	ubyte_t rsdp[];
} multiboot2_tag_old_rsdp_t;

typedef multiboot2_tag_old_rsdp_t multiboot2_tag_new_rsdp_t;

void* find_multiboot2_tag ( void* mbi, u32 type );

#endif /* MULTIBOOT2_H */
