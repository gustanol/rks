#ifndef _KERNEL_BOOT_ACPI_H
#define _KERNEL_BOOT_ACPI_H

#include <stddef.h>
#include <stdint.h>

/* Information structure about RSDP */
struct rsdp_info
{

	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint64_t rsdt_address;

	/* ACPI 2.0+ */
	uint32_t len;
	uint64_t xsdt_address;
	uint8_t extended_checksum;
	uint8_t reserved[3];
} __attribute__ ( ( packed ) );

#endif /* _KERNEL_BOOT_ACPI_H */
