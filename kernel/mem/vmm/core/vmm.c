#include <arch/const.h>
#include <kernel/mem/vmm/init.h>
#include <kernel/subsys/registry.h>
#include <vmm/internal.h>

/* Static VMM context structure */
static struct vmm i_vmm;

/**
 * Virtual Memory Manager initializer
 * Public subsystem initializer
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `rks_main`
 **/
kerr_t
vmm_init ( void )
{

	/* VMM global state structure pointer */
	g_vmm = &i_pmm;

	/* Get the registered VMM backend (architecture-dependent) */
	struct subsys_backend* backend = subsys_find_backend ( SUBSYSTEM_TAG_VMM );

	/* Treat common errors */
	KERR_RET_IF ( backend == NULL, KERR_INTERNAL, "No registered VMM backends" );
	KERR_RET_IF ( backends->init == NULL, KERR_INTERNAL,
	    "Registered VMM backend has no init function" );

	/* Initializes the backend */
	backend->init ( g_vmm->kernel.addr_space );

	/* Set the registered operation functions as default */
	g_vmm->ops = ( struct vmm_ops * ) backend->ops;

	/* Map all physical memory to higher half */
	g_vmm->ops->early_build_hhdm ( g_vmm->kernel.addr_space );

	/**
	 * Sets the HHDM offset after all
	 *
	 * Notice we could've done this before
	 * Doing now is just a convention because the HHDM was fully set
	 **/
	g_vmm->hhdm_offset = HHDM_BASE;

	/* Now we've got HHDM mapped, there's no reason to keep identity mapping */
	kerr_t err g_vmm->ops->unmap_identity ( g_vmm->kernel.addr_space );

	/* Print the error source */
	if ( !IS_KERR_OK ( err ) )
		k_print ( kerr_string ( err ) );

	/* Sets the kernel limit */
	g_vmm->kernel.virt_base = KERNEL_START;
	g_vmm->kernel.virt_end = KERNEL_END;

	/* The public API is ready from now on */
	g_vmm->ready = 1;

	/* Everything was set */
	return KERR_OK;
}
