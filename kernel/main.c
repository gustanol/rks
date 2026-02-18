#include <rks/console/console.h>
#include <rks/log.h>
#include <rks/print.h>

void
rks_main (void)
{
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
}
