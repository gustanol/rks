#include <rks/print.h>
#include <rks/screen/__terminal.h>

extern void rks_main(void);

void rks_main(void)
{
	_terminal_init();
	kprintf("Testing printing, %s!\n", "1");
	kprintf("Testing printing, %s!\n", "2");
	kprintf("Testing printing, %s!\n", "3");
	kprintf("Testing printing, %s!\n", "4");
	kprintf("Testing printing, %s!\n", "5");
	kprintf("Testing printing, %s!\n", "6");
	kprintf("Testing printing, %s!\n", "7");
	kprintf("Testing printing, %s!\n", "8");
	kprintf("Testing printing, %s!\n", "9");
	kprintf("Testing printing, %s!\n", "10");
	kprintf("Testing printing, %s!\n", "11");
	kprintf("Testing printing, %s!\n", "12");
	kprintf("Testing printing, %s!\n", "13");
	kprintf("Testing printing, %s!\n", "14");
	kprintf("Testing printing, %s!\n", "15");
	kprintf("Testing printing, %s!\n", "16");

	kprintf("%s\n", "1");
	kprintf("%s\n", "1");
	kprintf("%s\n", "1");
	kprintf("%s\n", "1");
	kprintf("%s\n", "1");
	kprintf("%s\n", "1");
	kprintf("%s\n", "1");

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
