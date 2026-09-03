#include <kernel/boot/parser.h>
#include <kernel/mem/alloc/init.h>
#include <kernel/mem/pmm/init.h>
#include <kernel/mem/vmm/init.h>
#include <kernel/mem/vmm/map.h>
#include <kernel/terminal/init.h>
#include <stdbool.h>

/**
 * Kernel C main entry point
 * Public entry
 *
 * Return -> void (Shall not return)
 * Arguments:
 *   1. `struct loader_info* loader_info`: Used bootloader info context
 **/
void
main ( struct loader_info* loader_info )
{

	struct boot_info boot_info;
	kerr_t err;

	/* Parse and fill the boot info structure out */
	boot_parser ( loader_info, &boot_info );

	/* Initializes the Physical Memory Manager */
	err = pmm_init ( &boot_info.mmap );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "PMM couldn't be initialized" );

	/* Initializes the Virtual Memory Manager */
	err = vmm_init (  );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "VMM couldn't be initialized" );

	/* Initializes the Allocation subsystem */
	err = alloc_init (  );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "ALLOC couldn't be initialized" );

	/* Initializes the Terminal subsystem */
	err = terminal_init ( &boot_info.framebuffer );
	if ( !IS_RET_OK ( err ) )
		k_print ( "The Terminal subsystem couldn't be initialized" );

	/* sched_loop () */
	while ( true )
		__asm__ volatile ( "hlt" );
}
