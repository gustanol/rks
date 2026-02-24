#include <rks/kernel/console/console.h>
#include <rks/kernel/klog/log.h>
#include <rks/kernel/klog/print.h>
#include <rks/kernel/init.h>

extern init_fn_t __drivers_init_start;
extern init_fn_t __drivers_init_end;

static void
init_drivers (void)
{
	for (init_fn_t * ptr = &__drivers_init_start;
	     ptr < &__drivers_init_end; ptr++)
		(*ptr) ();
}

void
rks_main (void)
{
	init_drivers ();

	console_init ();
	_log_init ();

	kprintf ("Testing %% >[0xEB]{printing, %d}!\n", 1);
	kprintf ("Testing %p printing, %d, %s!\n", 20, "RKS");
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

	/* Interrupts tests */
	volatile int x = 1;
	volatile int y = 0;
	volatile int z = x / y;
	kprintf ("z=%d\n", z);
	/* */
}
