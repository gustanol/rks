#include <rks/kernel/console/console.h>
#include <rks/kernel/klog/log.h>
#include <rks/kernel/klog/print.h>
#include <rks/kernel/init.h>
#include <rks/kernel/mem/pmm.h>
#include <rks/kernel/mem/heap.h>

extern init_fn_t __drivers_init_start;
extern init_fn_t __drivers_init_end;

static void
init_drivers (void)
{
	for (init_fn_t * ptr = &__drivers_init_start;
	     ptr < &__drivers_init_end; ptr++)
		(*ptr) ();
}

/*
 * RKS main generic entry point function
 *
 * -> void (should not return)
 * @mb: Multiboot2 info struct base pointer
 */
void
rks_main (void* mbi)
{
	/* Call architecture-dependent code right here */
	arch_early_init ();

	/* Init all self-registered drivers */
	init_drivers ();
	console_init ();
	_log_init ();

	pmm_init (mbi);
	heap_init ();

	test_pmm ();
	test_heap ();

	do
	{
		__asm__ volatile ("hlt");
	} while (1);
}
