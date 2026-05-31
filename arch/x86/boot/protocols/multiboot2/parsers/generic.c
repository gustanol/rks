#include <rks/kernel/klog/print.h>
#include <rks/kernel/klog/log.h>
#include <rks/lib/boot/multiboot2.h>
#include <x86/boot/loader_parser.h>
#include <x86/mem/early_alloc.h>

/* Prototypes
 * ========== */
static void __parse_mmap ( struct multiboot2_tag_mmap* tag,
                           struct mmap_tag* mmap );
static void __parse_framebuffer ( struct multiboot2_tag_framebuffer* tag,
                                  struct framebuffer_info* framebuffer );

/**
 * Parses the Multiboot2 information structure
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `vaddr_t info_ptr`: MBI2 pointer
 *   2. `struct boot_info* boot_info`: Boot info struct pointer
 * Callers:
 *   - `parse_boot`
 **/
void
_parse_mbi2 ( vaddr_t info_ptr, struct boot_info* boot_info )
{
	/* Gets the MB2 MMAP tag from the information pointer */
	struct multiboot2_tag_mmap* mb2_mmap =
	        ( struct multiboot2_tag_mmap * )
	        find_multiboot2_tag ( ( void * ) info_ptr,
	                              MULTIBOOT2_TAG_TYPE_MMAP );

	/* Allocates memory for the MMAP tag */
	boot_info->mmap =
	        ( struct mmap_tag * ) _early_alloc ( sizeof ( struct mmap_tag ),
	                                             0x10, FILL_ZEROS );

	/* MMAP parsing */
	__parse_mmap ( mb2_mmap, boot_info->mmap );

	/* === */

	/* Gets the command line */
	struct multiboot2_tag_cmdline* mb2_cmdline =
	        ( struct multiboot2_tag_cmdline * )
	        find_multiboot2_tag ( ( void * ) info_ptr,
	                              MULTIBOOT2_TAG_TYPE_CMDLINE );

	/* Copies the command line */
	boot_info->cmdline = mb2_cmdline->string;

	/* === */

	/* We will try both versions */
	struct multiboot2_tag_old_rsdp* mb2_acpi_old;
	struct multiboot2_tag_new_rsdp* mb2_acpi_new;

	/* Tries with the new ACPI version first */
	mb2_acpi_new =
	        ( struct multiboot2_tag_new_rsdp * )
	        find_multiboot2_tag ( ( void * ) info_ptr,
	                              MULTIBOOT2_TAG_TYPE_ACPI_NEW );

	/* It it's NULL, the new version is not available */
	if ( mb2_acpi_new == NULL )
		mb2_acpi_old =
	                ( struct multiboot2_tag_old_rsdp * )
	                find_multiboot2_tag ( ( void * ) info_ptr,
	                                      MULTIBOOT2_TAG_TYPE_ACPI_OLD );

	/* RSDP memory allocation */
	boot_info->rsdp =
	        ( struct rsdp_info * )
	        _early_alloc ( sizeof ( struct rsdp_info ), 0x10, FILL_ZEROS );

	/* Uses the version we got */
	boot_info->rsdp =
	        mb2_acpi_new ==
	        NULL ? ( struct rsdp_info * ) mb2_acpi_old : ( struct rsdp_info
	                                                       * ) mb2_acpi_new;

	/* Framebuffer memory allocation */
	boot_info->framebuffer =
	        ( struct framebuffer_info * )
	        _early_alloc ( sizeof ( struct framebuffer_info ), 0x10,
	                       FILL_ZEROS );

	/* Loggings
	   kprintf ( "[ MB2 ] Memory mapping gotten from the Multiboot2 info struct\n" );
	   kprintf ( "[ MB2 ] Cmdline received: \"%s\"\n", boot_info->cmdline );
	   kprintf ( "[ MB2 ] RSDP pointer: %p\n", boot_info->rsdp );
	 */
}
