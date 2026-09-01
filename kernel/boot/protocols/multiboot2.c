#include <boot/vendor/protocols/multiboot2.h>
#include <string.h>

/**
 * Multiboot2 protocol tag finder
 * Private helper
 *
 * Return `void *`: Tag base address pointer
 * Arguments:
 *   1. `void* info_ptr`: Multiboot2 info struct base pointer 
 *   2. `multiboot2_uint32_t type`: Type to be found
 **/
#define MULTIBOOT2_FIND_TAG \
	void* _multiboot2_find_tag ( void* info_ptr, multiboot2_uint32_t type )

/* Static prototypes */
static MULTIBOOT2_FIND_TAG;

void
multiboot2_parser ( uint64_t info_ptr, struct boot_info* boot )
{

	struct multiboot2_tag_mmap* multiboot2_mmap_tag;
	struct multiboot2_tag_string* multiboot2_cmdline_tag;
	struct multiboot2_tag_new_acpi* multiboot2_new_acpi_tag;
	struct multiboot2_tag_old_acpi* multiboot2_old_acpi_tag;
	struct multiboot2_tag_framebuffer* multiboot2_framebuffer_tag;

	/* Get the MMAP starter tag */
	multiboot2_mmap_tag =
	    ( struct multiboot2_tag_mmap * ) _multiboot2_find_tag ( ( void * ) info_ptr,
	    MULTIBOOT2_TAG_TYPE_MMAP );

	/* Do not continue if it's NULL */
	if ( multiboot2_mmap_tag != NULL )
		_multiboot2_parse_mmap ( multiboot2_mmap_tag, &boot_info->mmap );

	/* Gets the passed command line */
	multiboot2_cmdline_tag =
	    ( struct multiboot2_tag_string * ) _multiboot2_find_tag ( ( void * ) info_ptr,
	    MULTIBOOT2_TAG_TYPE_CMDLINE );

	/* Only sets it if the tag was correctly found */
	if ( multiboot2_cmdline_tag != NULL )
	{
		size_t cmdline_len = strlen ( multiboot2_cmdline_tag->string );
		boot_info->cmdline = early_alloc ( sizeof ( char ) * cmdline_len );
		strcpy ( *boot_info->cmdline, multiboot2_cmdline_tag->string );
	}

	/* Tries to get the new ACPI version */
	multiboot2_new_acpi_tag =
	    ( struct multiboot2_tag_new_acpi * ) _multiboot2_find_tag ( ( void * ) info_ptr,
	    MULTIBOOT2_TAG_TYPE_ACPI_NEW );

	/* If it fails, tries to get the old one */
	if ( multiboot2_new_acpi_tag == NULL )
	{
		multiboot2_old_acpi_tag =
		    ( struct multiboot2_tag_old_acpi * ) _multiboot2_find_tag ( ( void * ) info_ptr,
		    MULTIBOOT2_TAG_TYPE_ACPI_OLD );

		if ( multiboo2_old_acpi_tag != NULL )
			_multiboot2_parse_acpi ( multiboot2_old_acpi_tag, &boot_info->rsdp, false );
	} else
		/* REMIND: `true`: it is new */
		_multiboot2_parse_acpi ( multiboot2_new_acpi_tag, &boot_info->rsdp, true );

	/* Get the framebuffer tag */
	multiboot2_framebuffer_tag =
	    ( struct multiboot2_tag_framebuffer * ) _multiboot2_find_tag ( ( void * ) info_ptr,
	    MULTIBOOT2_TAG_TYPE_FRAMEBUFFER );

	/* If it was found, parses it */
	if ( multiboot2_framebuffer_tag != NULL )
		_multiboot2_parse_framebuffer ( multiboot2_framebuffer_tag, &boot_info->framebuffer );
}

/**
 * Internal tag finder
 *
 * Signature macro
 * Interface defined in this file
 **/
MULTIBOOT2_FIND_TAG
{

	/* Skip the Multiboot2 header to start searching for the tag */
	struct multiboot2_tag* tag =
	    ( struct multiboot2_tag * ) ( ( multiboot2_uint8_t ) info_ptr + MULTIBOOT2_HEADER_ALIGN );

	/* Do not stop until it's the end */
	for ( ; tag->type != MULTIBOOT2_TAG_TYPE_END; )
	{

		/* That's the type */
		if ( tag->type == type )
			return ( void * ) tag;

		/* Otherwise, increments the tag address and test it again in this loop */
		multiboot2_uint64_t next_tag_addr = ( multiboot2_uint64_t ) tag + tag->size;
		tag = ( struct multiboot2_tag * ) ( ( next_tag_addr + 7 ) & ~7 );   /* 8-byte aligned */
	}

	/* If it was not found, return NULL */
	return NULL;
}
