#include <rks/kernel/console/console.h>
#include <rks/kernel/klog/log.h>
#include <rks/kernel/klog/print.h>
#include <rks/kernel/init.h>
#include <rks/kernel/mem/pmm.h>

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

	kprintf ("Testing %% >[0xEB]{printing, %d}!\n", 1);
	kprintf ("Testing %p printing, %d, %s!\n", mbi, 20, "RKS");
	kprintf ("Testing printing, %o, %s!\n", 343, "Test");
	kprintf ("Testing printing, >[0xE9]{%o}!\n", 43034);

	kprintf ("%b\n", 5);
	kprintf ("%b\n", 0xAB);
	kprintf ("%b\n", 0xFFFF);

	pr_log (LOG_ERR, "\nx=%d\n", 7);

	INF ("mm: initialized\n");
	DBG ("something happened\n");
	WRN ("not right, but survivable\n");
	ERR ("error\n");

	do
	{
		__asm__ volatile ("hlt");
	} while (1);
}
