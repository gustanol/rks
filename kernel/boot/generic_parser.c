#include <boot/vendor/protocols/multiboot2.h>
#include <kernel/boot/parser.h.h>

/**
 * Bootloader-received information structure parser
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct loader_info* loader`: Bootloader state context
 *   2. `struct boot_info* boot`: Generic boot information structure
 * Callers:
 *   - `main`
 **/
void
boot_parser ( struct loader_info* loader, struct boot_info* boot )
{

	/* Conditional switch according the magic number */
	switch ( loader->magic )
	{
			case MULTIBOOT2_BOOTLOADER_MAGIC:
				multiboot2_parser ( loader->info_ptr, boot );
	};
}
