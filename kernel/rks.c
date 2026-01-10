extern void rks_main(void);

void rks_main(void)
{
	char       *video = (char *)0xB8000;
	const char *msg   = "RKS! From 64-bit!\n";

	for (int i = 0; msg[i] != '\0'; i++)
	{
		video[i * 2]     = msg[i];
		video[i * 2 + 1] = 0x0F;
	}

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
