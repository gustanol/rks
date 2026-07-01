#include <arch/const.h>
#include <pmm/internal.h>
#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/pmm/init.h>
#include <rks/kernel/mem/vmm/types.h>
#include <rks/kernel/subsys/registry.h>

/* Static prototypes
 * ================= */
static void _pmm_fill_total_memory ( struct mmap_tag* mmap );

/* Static PMM context structure */
static struct pmm i_pmm;

/**
 * Physical Memory Allocation context initializer
 * Global init function
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct mmap_tag* mmap`: Memory descriptors entries
 * Callers:
 *   - `rks_main`
 **/
void
pmm_init ( struct mmap_tag* mmap )
{
	/* We can't continue if it's NULL */
	if ( mmap == NULL )
	{
		/*
		   k_panic ( "mmap == NULL" );
		 */
	}

	/* Sets the global pointer */
	g_pmm = &i_pmm;

	/* Total memory size in bytes */
	g_pmm->memory.total = 0;

	/* First loop to get all available memory */
	for ( u32 i = 0; i < mmap->entry_count; i++ )
	{
		/* Only get available memory */
		if ( mmap->entries[i].type == MEM_AVAILABLE )
		{
			/* Total memory */
			g_pmm->memory.total += mmap->entries[i].len;
		}
	}

	/* The amount of physical frames the memory has */
	g_pmm->frames.total = g_pmm->memory.total / FRAME_SIZE;
	g_pmm->frames.used = 0;

	/* Gets the registered backend */
	struct subsys_backend* backend = subsys_find_backend ( SUBSYS_TAG_PMM );

	/*
	   if (!backend)
	   k_panic("no registered backend");
	 */

	/*
	   if (!backend->init)
	   k_panic("no init function");
	 */

	/*
	   k_print("[PMM] Registered backend: '%s'\n", backend->name);
	 */

	/* Backend init function
	 * ===================== */

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

	/* Marks available entries as free regions
	 * ======================================= */
	for ( u32 i = 0; i < mmap->entry_count; i++ )
	{
		/* Available memory entries */
		if ( mmap->entries[i].type == MEM_AVAILABLE )
		{
			/* We can mark this region as free to use */
			g_pmm->ops->free_region ( mmap->entries[i].addr,
			                          mmap->entries[i].len );
		}
	}

	/* MARKS THE WHOLE MAPPING, NOT JUST ISOLATED REGIONS (LATER) */

	g_pmm->ops->fill_region ( ( paddr_t )
	                          ( ( vaddr_t ) KERNEL_START -
	                            ( vaddr_t ) HHDM_BASE ),
	                          ( vaddr_t ) KERNEL_END -
	                          ( vaddr_t ) KERNEL_START );

	/* Logging
	   kprintf ( "PMM: Available memory = %llu MiB\n",
	   g_pmm->memory.total / 1024 / 1024 );
	   kprintf ( "PMM: Total frames = %lu\n", g_pmm->frames.total );
	   kprintf ( "PMM: Used frames = %lu\n", g_pmm->frames.used );
	 */

	/* OK */
	return KERR_OK;
}
