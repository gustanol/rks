#include <rks/print.h>
#include <rks/screen/__terminal.h>

extern void rks_main(void);

void rks_main(void)
{
	_terminal_init();
	kprintf("Testing printing, %d!\n", 1);
	kprintf("Testing printing, %d, %s!\n", 20, "RKS");
	kprintf("Testing printing, %d, %s!\n", 343, "Test");
	kprintf("Testing printing, %d!\n", 43034);

	kprintf("%x\n", 12);
	kprintf("%x\n", 0xAB);
	kprintf("%x\n", 0xFFFF);

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
