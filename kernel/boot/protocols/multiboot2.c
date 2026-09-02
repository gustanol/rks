#include <arch/mem/early_alloc.h>
#include <boot/vendor/protocols/multiboot2.h>
#include <string.h>

/**
 * Multiboot2 protocol tag finder
 * Private helper
 *
 * Return `void *`: Tag base address pointer
 * Arguments:
 *   1. `multiboot2_uint64_t info_ptr`: Multiboot2 info struct base address 
 *   2. `multiboot2_uint32_t type`: Type to be found
 **/
#define MULTIBOOT2_FIND_TAG \
	void* _multiboot2_find_tag ( multiboot2_uint64_t info_ptr, multiboot2_uint32_t type )

/**
 * Multiboot2 protocol MMAP parser 
 * Private helper
 *
 * Return `void`
 * Arguments:
 *   1. `struct multiboot2_tag_mmap* mmap_tag`: Gotten Multiboot2 MMAP tag
 *   2. `struct mmap_tag* boot_mmap`: Generic MMAP tag type
 **/
#define MULTIBOOT2_PARSE_MMAP \
	void _multiboot2_parse_mmap ( struct multiboot2_tag_mmap* mmap_tag, struct mmap_tag* boot_mmap )

/**
 * Multiboot2 protocol Framebuffer parser 
 * Private helper
 *
 * Return `void`
 * Arguments:
 *   1. `struct multiboot2_tag_framebuffer* framebuffer_tag`: Gotten Multiboot2 Framebuffer tag
 *   2. `struct framebuffer_info* boot_framebuffer`: Generic Framebuffer tag type
 **/
#define MULTIBOOT2_PARSE_FRAMEBUFFER \
	void _multiboot2_parse_framebuffer ( \
		struct multiboot2_tag_framebuffer* framebuffer_tag, \
		struct framebuffer_info* boot_framebuffer \
	)

/* Static prototypes */
static MULTIBOOT2_FIND_TAG;
static MULTIBOOT2_PARSE_MMAP;
static MULTIBOOT2_PARSE_FRAMEBUFFER;

/**
 * Multiboot2 info structure parser
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `uint64_t info_ptr`: Multiboot2 info struct base pointer
 *   2. `struct boot_info* boot`: Generic boot info
 * Callers:
 *   - `boot_parser`
 **/
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
	    ( struct multiboot2_tag_mmap * ) _multiboot2_find_tag ( info_ptr,
	    MULTIBOOT2_TAG_TYPE_MMAP );

	/* Do not continue if it's NULL */
	if ( multiboot2_mmap_tag != NULL )
		_multiboot2_parse_mmap ( multiboot2_mmap_tag, &boot->mmap );

	/* Gets the passed command line */
	multiboot2_cmdline_tag =
	    ( struct multiboot2_tag_string * ) _multiboot2_find_tag ( info_ptr,
	    MULTIBOOT2_TAG_TYPE_CMDLINE );

	/* Only sets it if the tag was correctly found */
	if ( multiboot2_cmdline_tag != NULL )
	{
		size_t cmdline_len = strlen ( multiboot2_cmdline_tag->string );
		boot->cmdline = early_alloc ( sizeof ( char ) * cmdline_len );
		strcpy ( *boot->cmdline, multiboot2_cmdline_tag->string );
	}

	/* Tries to get the new ACPI version */
	multiboot2_new_acpi_tag =
	    ( struct multiboot2_tag_new_acpi * ) _multiboot2_find_tag ( info_ptr,
	    MULTIBOOT2_TAG_TYPE_ACPI_NEW );

	/* If it fails, tries to get the old one */
	if ( multiboot2_new_acpi_tag == NULL )
	{
		multiboot2_old_acpi_tag =
		    ( struct multiboot2_tag_old_acpi * ) _multiboot2_find_tag ( info_ptr,
		    MULTIBOOT2_TAG_TYPE_ACPI_OLD );

		if ( multiboot2_old_acpi_tag != NULL )
			boot->rsdp = ( struct rsdp_info * ) multiboot2_old_acpi_tag->rsdp;
	} else
		boot->rsdp = ( struct rsdp_info * ) multiboot2_new_acpi_tag->rsdp;

	/* Get the framebuffer tag */
	multiboot2_framebuffer_tag =
	    ( struct multiboot2_tag_framebuffer * ) _multiboot2_find_tag ( info_ptr,
	    MULTIBOOT2_TAG_TYPE_FRAMEBUFFER );

	/* If it was found, parses it */
	if ( multiboot2_framebuffer_tag != NULL )
		_multiboot2_parse_framebuffer ( multiboot2_framebuffer_tag, &boot->framebuffer );
}

/**
 * Internal MMAP parser 
 *
 * Signature macro
 * Interface defined in this file
 **/
MULTIBOOT2_PARSE_MMAP
{

	/* Ignores the first 16 bytes of this tag and count only from `entries` */
	boot_mmap->entry_count = ( mmap_tag->size - 16 ) / mmap_tag->entry_size;

	/* Allocates the required amount of entries */
	boot_mmap->entries =
	    ( struct mmap_entry * ) early_alloc ( sizeof ( struct mmap_entry ) *
	    boot_mmap->entry_count );

	/* Iterates for every entry */
	for ( multiboot2_uint32_t i = 0; i < boot_mmap->entry_count; i++ )
	{

		/* Get the current Multiboot2 MMAP entry */
		struct multiboot2_mmap_entry* entry = &mmap_tag->entries[i];

		/* Fills the current boot MMAP entry with this entry */
		boot_mmap->entries[i].addr = entry->addr;   /* Physical addresss */
		boot_mmap->entries[i].len = entry->len; /* Map length */
		boot_mmap->entries[i].type = entry->type;   /* MMAP type (enum mem_type) */
	}
}

/**
 * Internal Framebuffer parser 
 *
 * Signature macro
 * Interface defined in this file
 **/
MULTIBOOT2_PARSE_FRAMEBUFFER
{

	/* Common set */
	boot_framebuffer->addr = framebuffer_tag->common.framebuffer_addr;
	boot_framebuffer->pitch = framebuffer_tag->common.framebuffer_pitch;
	boot_framebuffer->width = framebuffer_tag->common.framebuffer_width;
	boot_framebuffer->height = framebuffer_tag->common.framebuffer_height;
	boot_framebuffer->bpp = framebuffer_tag->common.framebuffer_bpp;

	/* RGB specific configuration */
	if ( framebuffer_tag->common.framebuffer_type == MULTIBOOT2_FRAMEBUFFER_TYPE_RGB )
	{

		/* Red */
		boot_framebuffer->rgb.red.pos = framebuffer_tag->framebuffer_red_field_position;
		boot_framebuffer->rgb.red.mask = framebuffer_tag->framebuffer_red_mask_size;

		/* Green */
		boot_framebuffer->rgb.green.pos = framebuffer_tag->framebuffer_green_field_position;
		boot_framebuffer->rgb.green.mask = framebuffer_tag->framebuffer_green_mask_size;

		/* Blue */
		boot_framebuffer->rgb.blue.pos = framebuffer_tag->framebuffer_blue_field_position;
		boot_framebuffer->rgb.blue.mask = framebuffer_tag->framebuffer_blue_mask_size;
	}
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
	    ( struct multiboot2_tag * ) ( ( multiboot2_uint64_t ) ( ( multiboot2_uint8_t ) info_ptr +
	        8 ) );

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
