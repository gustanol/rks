#include <arch/mem/early_alloc.h>
#include <kernel/compiler/sections.h>
#include <lib/bitwise.h>

/* External symbols */
extern void BOOTSTRAP_ARENA_START ( void );
extern void BOOTSTRAP_ARENA_END ( void );

/* Next available address */
__BOOT_DATA u64 next_addr = ( u64 ) BOOTSTRAP_ARENA_START;

/*
 * Early boot allocator
 * Public helper
 *
 * Return -> `void *`: Allocated region base pointer
 * Arguments:
 *   1. `size_t size`: Region size
 *   2. `u32 align`: Alignment size
 * Callers:
 *   - Many
 */
__BOOT_TEXT void*
_early_alloc ( size_t size, u32 align )
{
	/* Current pointer */
	void* ptr = ( void * ) next_addr;

	/* Verify boundary crosses */
	if ( next_addr + size > ( u64 ) BOOTSTRAP_ARENA_END )
		return NULL;

	/* Update the pointer and align it up */
	next_addr += size;
	next_addr = ALIGN_UP ( next_addr, align );

	/* Return it */
	return ptr;
}
