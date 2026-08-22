#include <kernel/boot/info.h>
#include <kernel/mem/alloc/init.h>
#include <kernel/mem/pmm/init.h>
#include <kernel/mem/vmm/init.h>
#include <kernel/mem/vmm/map.h>
#include <kernel/terminal/init.h>

/* Statically allocated boot info struct */
static struct boot_info info;

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

	/* Parse and fill the boot info structure out */
	parse_boot ( loader_info, &boot_info );

	/* Initializes the Physical Memory Manager */
	err = pmm_init ( info.mmap );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "PMM couldn't be initialized" );

	/* Initializes the Virtual Memory Manager */
	err = vmm_init (  );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "VMM couldn't be initialized" );

	/**
	 * `info` is statically allocated in this file
	 * At that point, the only virtual mapping is identity mapping
	 *
	 * VMM is responsible to unmap identity mapping,
	 * Which means `info` now points to an unmapped address
	 *
	 * As we still need it, we can use the VMM public API to
	 * Get a virtual address from the current `info` physical address
	 **/
	vaddr_t new_boot_info_addr = vmm_phys_to_virt ( ( paddr_t ) info );
	info = new_boot_info_addr;

	/* Initializes the Allocation subsystem */
	err = alloc_init (  );
	if ( !IS_RET_OK ( err ) )
		k_panic ( "ALLOC couldn't be initialized" );

	/* Initializes the Terminal subsystem */
	err = terminal_init ( info.fb );
	if ( !IS_RET_OK ( err ) )
		k_print ( "The Terminal subsystem couldn't be initialized" );

	/* sched_loop () */
	while ( true )
		__asm__ volatile ( "hlt" );
}
