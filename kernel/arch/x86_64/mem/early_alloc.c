#include <arch/mem/early_alloc.h>
#include <arch/sections.h>
#include <lib/bitwise.h>

/* External symbols */
extern void BOOTSTRAP_ARENA_START ( void );
extern void BOOTSTRAP_ARENA_END ( void );

/* Next available arena address */
__BOOT_DATA uint64_t next_addr = ( uint64_t ) BOOTSTRAP_ARENA_START;

/**
 * Early boot allocator
 * Public helper
 *
 * Return -> `void *`: Allocated region base pointer
 * Arguments:
 *   1. `size_t size`: Region size
 * Callers:
 *   - Many
 **/
__BOOT_TEXT void*
early_alloc ( size_t size )
{

	/* Current pointer */
	void* ptr = ( void * ) next_addr;

	/* Verify boundary crosses */
	if ( next_addr + size > ( uint64_t ) BOOTSTRAP_ARENA_END )
		return NULL;

	/* Update the pointer and align it up (16 bits) */
	next_addr += size;
	next_addr = ALIGN_UP ( next_addr, 16 );

	/* Return it */
	return ptr;
}
