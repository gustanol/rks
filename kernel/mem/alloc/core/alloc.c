#include <alloc/internal.h>
#include <kernel/mem/alloc/init.h>

/* Statically allocate the global state Allocation context struct */
static struct alloc_ctx i_alloc;

/**
 * Allocation subsystem initializer
 * Public subsystem initializer
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `rks_main`
 **/
kerr_t
alloc_init ( void )
{

	/* Sets the pointer */
	g_alloc = &i_alloc;

	/* Create the kernel global heap context */
	struct heap* kernel_heap = _heap_create (  );

	/* Verify integrity */
	if ( IS_KERR ( kernel_heap ) )
		return GET_KERR ( kernel_heap );

	/* Defines it correctly */
	g_alloc->kernel.heap = kernel_heap;

	/*
	   struct slab* kernel_slab = _slab_create (  );

	   if ( IS_KERR ( kernel_slab ) )
	   return GET_KERR ( kernel_slab );

	   g_alloc->kernel.slab_ctx = kernel_slab;
	 */

	/* Ready to be used */
	g_alloc->ready = true;
}
