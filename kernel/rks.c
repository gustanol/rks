#include <rks/print.h>
#include <rks/screen/__terminal.h>

extern void rks_main(void);

void rks_main(void)
{
	_terminal_init();
	kprintf("Testing printing, %s! RKS!", "Gustavo");

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
