#include <boot/vendor/protocols/multiboot2.h>

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

/* Static prototypes */
static MULTIBOOT2_FIND_TAG;

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
