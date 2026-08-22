#ifndef _KERNEL_BOOT_LOADER_H
#define _KERNEL_BOOT_LOADER_H

#include <stddef.h>
#include <stdint.h>

/* Specific information about the kernel bootloader */
struct loader_info
{

	/* Bootloader magic number */
	uint32_t magic;

	/* Information pointer */
	uint64_t info_ptr;
} __attribute__ ( ( packed ) );

#endif /* _KERNEL_BOOT_LOADER_h */
