
/*
 * Main C kernel code point
 *
 * Define the initial kernel context
 *
 * The `rks_main` function, once reached, should not return to where it was called
 */

#include <rks/kernel/console/console.h>
#include <rks/kernel/klog/log.h>
#include <rks/kernel/klog/print.h>
#include <rks/kernel/init.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/mem/vmm.h>
#include <rks/kernel/mem/heap.h>
#include <rks/boot.h>

extern init_fn_t __drivers_init_start;
extern init_fn_t __drivers_init_end;

/* Statically allocated boot info struct */
static boot_info_t boot_info;

/*
 * Call every init function provided by every registered driver
 *
 * -> void
 */
static void
init_drivers ( void )
{
	for ( init_fn_t * ptr = &__drivers_init_start;
	      ptr < &__drivers_init_end; ptr++ )
		( *ptr ) (  );  /* Init function */
}

/*
 * Kernel main entry point
 *
 * -> void (should not return)
 * @loader_info: Pointer to a struct defining the bootloader
 *   - Only its `magic number` and a pointer to its `info struct`
 */
void
rks_main ( loader_info_t* loader_info )
{
	/* Call architecture-dependent code right here */
	arch_early_init (  );

	/* Init all self-registered drivers */
	init_drivers (  );

	console_init (  );
	_log_init (  );

	/* Generic layer to parse any info structure */
	parse_boot ( loader_info, &boot_info );

	/* Physical Memory Manager */
	pmm_init ( boot_info.mmap );
	heap_init (  );

	/* Virtual Memory Manager */
	vmm_init (  );

	do
	{
		__asm__ volatile ( "hlt" );
	} while ( true );
}
