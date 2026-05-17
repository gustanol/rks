#include <mem/early_alloc.h>
#include <rks/kernel/mem/vmm.h>
#include <rks/lib/bitwise.h>
#include <rks/lib/string.h>

/* Next available address */
vaddr_t next_addr = ( vaddr_t ) kernel_end;

/*
 * Early boot allocator
 * Public helper
 *
 * Return -> `void *`: Allocated region base pointer
 * Arguments:
 *   1. `size_t size`: Region size
 *   2. `u32 align`: Alignment size
 *   3. `enum alloc_flags alloc_flag`: Allocation flag to define how to fill the allocated region
 * Callers:
 *   - Many
 *
 * Others:
 *   - Used to allocate structures before the PMM is enabled
 */

void*
_early_alloc ( size_t size, u32 align, enum alloc_flags alloc_flag )
{
	/* Treats the pointer
	 * =========================== */

	/* Current pointer */
	void* ptr = ( void * ) next_addr;

	/* Updates the pointer and align it up */
	next_addr += size;
	next_addr = ALIGN_UP ( next_addr, align );
	/* =========================== */

	/* Fills the region if solicited
	 * ============================= */
	if ( alloc_flag == FILL_ZEROS )
		memset ( ptr, 0, size );

	else if ( alloc_flag == FILL_ONES )
		memset ( ptr, 1, size );
	/* ============================= */

	/* Returns the pointer */
	return ptr;
}
