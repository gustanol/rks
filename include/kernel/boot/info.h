#ifndef BOOT_H
#define BOOT_H

#include "fb.h"
#include "loader.h"
#include "mmap.h"
#include "rsdp.h"
#include <rks/kernel/types.h>

/* Generic boot information */
struct boot_info
{
	/* MMAP */
	struct mmap_tag* mmap;

	/* Firmware framebuffer */
	struct framebuffer_info* framebuffer;

	/* RSDP */
	struct rsdp_info* rsdp;

	/* Kernel command line */
	const char* cmdline;
};

/* Prototypes */
void parse_boot ( struct loader_info* loader, struct boot_info* boot );

#endif /* BOOT_H */
