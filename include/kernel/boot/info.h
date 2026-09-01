#ifndef _KERNEL_BOOT_INFO_H
#define _KERNEL_BOOT_INFO_H

#include "fb.h"
#include "loader.h"
#include "mmap.h"
#include "acpi.h"

/* Generic boot information */
struct boot_info
{
	/* MMAP */
	struct mmap_tag mmap;

	/* Firmware framebuffer */
	struct framebuffer_info framebuffer;

	/* RSDP */
	struct rsdp_info* rsdp;

	/* Kernel command line */
	char** cmdline;
};

#endif /* _KERNEL_BOOT_INFO_H */
