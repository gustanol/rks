#include <arch/const.h>
#include <rks/kernel/mem/vmm/init.h>
#include <rks/kernel/subsys/registry.h>
#include <vmm/internal.h>

/* Static VMM context structure */
static struct vmm i_vmm;

/**
 * Virtual Memory Manager initializer
 * Public subsystem initializer
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `rks_main`
 **/
void
vmm_init ( void )
{
	/* VMM global state structure pointer */
	g_vmm = &i_pmm;

	/* Gets the registered VMM backend (architecture-dependent) */
	struct subsys_backend* backend =
	        subsys_find_backend ( SUBSYSTEM_TAG_VMM );

	/*
	   if (!backend)
	   k_panic("no registered backend");
	 */

	/*
	   if (!backend->init)
	   k_panic("no registered backend");
	 */

	/* Initializes the backend */
	backend->init ( g_vmm->kernel.addr_space );

	/* Sets the registered operation functions as default */
	g_vmm->ops = ( struct vmm_ops * ) backend->ops;

	/* Maps all physical memory to higher half */
	g_vmm->ops->early_build_hhdm ( g_vmm->kernel.addr_space );

	/**
         * Sets the HHDM offset after all
         *
         * Notice we could've done this before
         * Doing now is just a convention because the HHDM was fully set
         **/
	g_vmm->hhdm_offset = HHDM_BASE;

	/* Now we've got HHDM mapped, there's no reason to keep identity mapping */
	g_vmm->ops->early_identity_unmap ( g_vmm->kernel.addr_space );

	/* Sets the kernel limit
	 * ===================== */
	g_vmm->kernel.virt_base = KERNEL_START;
	g_vmm->kernel.virt_end = KERNEL_END;

	/* Every is ready from now on */
	g_vmm->ready = 1;
}
