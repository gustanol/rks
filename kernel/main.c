#include <rks/console.h>
#include <rks/log.h>
#include <rks/print.h>

extern void rks_main (void);

void
rks_main (void)
{
	_terminal_init ();
	_log_init ();

	kprintf ("Testing %% printing, %d!\n", 1);
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
	INF ("test: just testing\n");

	while (1)
	{
		__asm__ volatile ("hlt");
	}
}
