#ifndef KERNEL_BOOT_LOADER_H
#define KERNEL_BOOT_LOADER_H

#include <rks/kernel/types.h>

/* Specific information about the kernel bootloader */
struct loader_info
{
	/* Bootloader magic number */
	u32 magic;

	/* Information pointer */
	addr_t info_ptr;
} __attribute__ ( ( packed ) );

#endif /* KERNEL_BOOT_LOADER_h */
