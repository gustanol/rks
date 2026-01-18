#include <rks/log.h>
#include <rks/print.h>
#include <rks/screen/__terminal.h>

extern void rks_main(void);

void rks_main(void)
{
	_terminal_init();
	_log_init();

	kprintf("%[0xAF]{Testing} %% printing, %d!\n", 1);
	kprintf("Testing %p printing, %d, %s!\n", 20, "RKS");
	kprintf("Testing printing, %o, %s!\n", 343, "Test");
	kprintf("Testing printing, %o!\n", 43034);

	kprintf("%b\n", 5);
	kprintf("%b\n", 0xAB);
	kprintf("%b\n", 0xFFFF);

	pr_log(LOG_ERR, "x=%d\n", 7);

	INF("mm: initialized\n", 1);
	DBG("something happened\n", 2);
	WRN("not right, but survivable\n", 2);
	ERR("error\n", 2);

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
