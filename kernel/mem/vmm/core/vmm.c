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
	g_vmm = &i_vmm;

	/* Get the registered VMM backend (architecture-dependent) */
	struct subsys_backend* backend = subsys_find_backend ( SUBSYS_TAG_VMM );

	/* Treat common errors */
	KERR_RET_IF ( backend == NULL, KERR_INTERNAL, "No registered VMM backends" );
	KERR_RET_IF ( backend->init == NULL, KERR_INTERNAL,
	    "Registered VMM backend has no init function" );

	/* Specific VMM backend context */
	struct vmm_backend* vmm_backend = vmm_backend_from_base ( backend );

	/* Initializes the backend */
	vmm_backend->init ( g_vmm->kernel.addr_space );

	/* Set the registered operation functions as default */
	g_vmm->ops = ( struct vmm_ops * ) vmm_backend->ops;

	/* Map all physical memory to higher half */
	g_vmm->ops->early_build_hhdm ( g_vmm->kernel.addr_space );

	/**
	 * Sets the HHDM offset after all
	 *
	 * Notice we could've done this before
	 * Doing now is just a convention because the HHDM was fully set
	 **/
	g_vmm->hhdm_offset = ( vaddr_t ) HHDM_BASE;

	/* Now we've got HHDM mapped, there's no reason to keep identity mapping */
	kerr_t err g_vmm->ops->unmap_identity ( g_vmm->kernel.addr_space );

	/* Print the error source */
	if ( !IS_RET_OK ( err ) )
		k_print ( kerr_str ( err ) );

	/* Sets the kernel limit */
	g_vmm->kernel.virt_base = ( vaddr_t ) KERNEL_START;
	g_vmm->kernel.virt_end = ( vaddr_t ) KERNEL_END;

	/* The public API is ready from now on */
	g_vmm->ready = 1;

	/* Everything was set */
	return KERR_OK;
}
