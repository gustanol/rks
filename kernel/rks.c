#include <rks/print.h>
#include <rks/screen/__terminal.h>

extern void rks_main(void);

void rks_main(void)
{
	_terminal_init();
	kprintf("Testing %% printing, %d!\n", 1);
	kprintf("Testing %p printing, %d, %s!\n", 20, "RKS");
	kprintf("Testing printing, %o, %s!\n", 343, "Test");
	kprintf("Testing printing, %o!\n", 43034);

	kprintf("%b\n", 5);
	kprintf("%b\n", 0xAB);
	kprintf("%b\n", 0xFFFF);

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
