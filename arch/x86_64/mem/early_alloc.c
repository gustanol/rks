#include <mem/early_alloc.h>
#include <rks/kernel/mem/vmm.h>
#include <rks/lib/bitwise.h>

/* Next available address */
paddr_t next_addr = ( paddr_t ) kernel_end;

/*
 * Early boot allocator
 * Used to allocate structures before the PMM is enabled
 *
 * -> void *: Base pointer to the reserved memory
 * @size: Object size to be allocated
 */

void*
early_alloc ( size_t size )
{
	/* Treat the pointer */
	/* =========================== */

	/* Current pointer */
	void* ptr = ( void * ) next_addr;

	/* Update the pointer and align it up */
	next_addr += size;
        next_addr = ALIGN_UP(next_addr, 0x10);
	/* =========================== */

	return ptr;
}
