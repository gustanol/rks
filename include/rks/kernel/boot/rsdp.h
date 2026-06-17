#ifndef KERNEL_BOOT_RSDP_H
#define KERNEL_BOOT_RSDP_H

#include <rks/kernel/types.h>

/* Information structure about RSDP */
struct rsdp_info
{
	char signature[8];
	ubyte_t checksum;
	char oemid[6];
	ubyte_t revision;
	addr_t rsdt_address;

	/* ACPI 2.0+ */
	u32 len;
	addr_t xsdt_address;
	ubyte_t extended_checksum;
	ubyte_t reserved[3];
} __attribute__ ( ( packed ) );

#endif /* KERNEL_BOOT_RSDP_H */
