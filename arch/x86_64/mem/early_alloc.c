#include <mem/early_alloc.h>
#include <rks/kernel/mem/vmm.h>

/* From linker */
extern u8 kernel_end[];

/* Next available address */
vaddr_t next_addr = ( vaddr_t ) kernel_end;

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

	/* Update the pointer */
	next_addr += size;
	/* =========================== */

	return ptr;
}
