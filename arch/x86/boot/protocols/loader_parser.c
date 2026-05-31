#include <rks/boot.h>
#include <rks/lib/boot/multiboot2.h>
#include <x86/boot/loader_parser.h>

/*
 * Parses the bootloader-given Multiboot2 information structure
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct loader_info* loader`: Bootloader state context
 *   2. `struct boot_info* booti`: Generic boot information structure
 * Callers:
 *   - `rks_main`
 */
void
parse_boot ( struct loader_info* loader, struct boot_info* boot )
{
	/* Loaded by a Multibot2 bootloader */
	if ( loader->magic == MULTIBOOT2_BOOTLOADER_MAGIC )
		parse_mbi2 ( loader->info_ptr, boot );
}
