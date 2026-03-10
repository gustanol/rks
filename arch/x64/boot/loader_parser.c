#include <rks/boot.h>
#include <boot/loader_parser.h>

/*
 * Helper used to pass an info struct pointer (given by the loader)
 * To the correct parser (according the magic number)
 *
 * -> void
 * @loader_info: Pointer to the struct containing the bootloader info
 * @boot_info: Pointer to the generic boot info
 */
void
parse_boot (loader_info_t* loader_info, boot_info_t* boot_info)
{
	/* Loaded by a Multibot2 bootloader */
	if (loader_info->magic == MULTIBOOT2_BOOTLOADER_MAGIC)
		parse_mbi2 (loader_info->info_ptr, boot_info);
}
