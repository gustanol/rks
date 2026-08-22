#include <arch/const.h>
#include <pmm/internal.h>
#include <kernel/kerr/kerr.h>
#include <kernel/mem/pmm/frames.h>
#include <kernel/mem/pmm/init.h>
#include <kernel/mem/vmm/types.h>
#include <kernel/subsys/registry.h>

/* Static prototypes */
static void _pmm_fill_total_memory ( struct mmap_tag* mmap );

/* Static PMM context structure */
static struct pmm i_pmm;

/**
 * Physical Memory Allocation context initializer
 * Subsystem init function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct mmap_tag* mmap`: Memory descriptors entries
 * Callers:
 *   - `rks_main`
 **/
kerr_t
pmm_init ( struct mmap_tag* mmap )
{

	/* We can't continue if `mmap` isn't valid */
	KERR_RET_IF ( mmap == NULL, KERR_INVAL, "MMAP pointer is NULL" );
	KERR_PROPAGATE ( mmap );

	/* Sets the global pointer */
	g_pmm = &i_pmm;

	/* Total memory size in bytes */
	g_pmm->memory.total = 0;

	/* Guarantee `mmap->entry` is usable */
	KERR_RET_IF ( mmap->entry == NULL, KERR_NOTMAPPED, "Not valid MMAP entries" );
	KERR_PROPAGATE ( mmap->entry );

	/* It's already unsigned */
	KERR_RET_IF ( mmap->entry_count == 0, KERR_INTERNAL, "Zero MMAP entries count" );

	/* First loop to get all available memory */
	for ( u32 i = 0; i < mmap->entry_count; i++ )

	    /* Only get available memory */
		if ( mmap->entries[i].type == MEM_AVAILABLE )
			g_pmm->memory.total += mmap->entries[i].len;

	/* Amount of physical frames the memory has */
	g_pmm->frames.total = FRAMES_BY_SIZE ( g_pmm->memory.total );
	g_pmm->frames.used = 0;

	/**
         * Get the registered backend
         *
         * Note it should be only one
         * For any reason, if we get more than one registered backend,
         * The first one found will be returned
         **/
	struct subsys_backend* backend = subsys_find_backend ( SUBSYS_TAG_PMM );

	/* Check integrity */
	KERR_RET_IF ( backend == NULL, KERR_INTERNAL, "No registered PMM backend" );
	KERR_RET_IF ( backend->init == NULL, KERR_INTERNAL,
	    "[%s] backend has no init function", backend->name );

	/* Context (Useful non-specific data) */
	struct pmm_init_backend_ctx ctx = {
		.mem_total = g_pmm->memory.total,
		.frames_total = g_pmm->frames.total,
	};

	/* Calls it */
	backend->init ( &ctx );

	/*
	 * NOTE: For now on, the backend should be correctly initialized
	 * It means we can securely use the function pointers
	 *
	 * Sets the operations right here
	 */
	g_pmm->ops = ( struct pmm_ops * ) backend->ops;

	/* Marks available entries as free regions */
	for ( u32 i = 0; i < mmap->entry_count; i++ )

	    /* Available memory entries */
		if ( mmap->entries[i].type == MEM_AVAILABLE )

		    /* We can mark this region as free to use */
			g_pmm->ops->free_region ( mmap->entries[i].addr, mmap->entries[i].len );

	/**
         * Mark the whole kernel size as used
         * Note this already includes bootstrap region
         **/
	g_pmm->ops->fill_region ( KERNEL_START - KERNEL_VIRT_OFF,
	    KERNEL_END - KERNEL_START, g_pmm->frame.total );

	/* Already to be used by dispatchers */
	g_pmm->ready = 1;

	/* Logging
	   kprintf ( "PMM: Available memory = %llu MiB\n",
	   g_pmm->memory.total / 1024 / 1024 );
	   kprintf ( "PMM: Total frames = %lu\n", g_pmm->frames.total );
	   kprintf ( "PMM: Used frames = %lu\n", g_pmm->frames.used );
	 */

	/* PMM correct intialized */
	return KERR_OK;
}
