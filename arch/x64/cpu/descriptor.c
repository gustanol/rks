#include "descriptor.h"

extern void _setup_descriptors (void);

#define GDT_ENTRIES 7
#define LOAD_GDT(descriptor) \
    __asm__ volatile ("lgdt %0" : : "m"(*descriptor) : "memory")

struct gdt_e gdt[GDT_ENTRIES] __attribute__ ((aligned (16)));
static struct tss_t tss;

static void __set_gdt_entries (void);

void
_setup_descriptors (void)
{
	static struct gdt_d gdt_desc __attribute__ ((aligned (16)));

	__set_gdt_entries ();

	gdt_desc.gdt_limit = (GDT_ENTRIES * sizeof (gdt[0])) - 1;
	gdt_desc.gdt_start = (__u64) & gdt;

	LOAD_GDT (&gdt_desc);
}

void
__set_gdt_entries (void)
{
	/* Null entry */
	gdt[0] = (struct gdt_e)
	{ 0 };

	/* Kernel code segment */
	gdt[1] = (struct gdt_e)
	{
		.limit_l = 0xFFFF,
		.base_l = 0x0000,
		.base_m = 0x00,
		.access_byte = 0x9A,	/* 0b10011010 */
		.flags_limit = 0x20 | 0x0F,
		.base_u = 0x00,
	};

	/* Kernel data segment */
	gdt[2] = (struct gdt_e)
	{
		.limit_l = 0xFFFF,
		.base_l = 0x0000,
		.base_m = 0x00,
		.access_byte = 0x92,	/* 0b10010010 */
		.flags_limit = 0x00 | 0x0F,
		.base_u = 0x00,
	};

	/* User code segment */
	gdt[3] = (struct gdt_e)
	{
		.limit_l = 0xFFFF,
		.base_l = 0x0000,
		.base_m = 0x00,
		.access_byte = 0xFA,	/* 0b11111010 */
		.flags_limit = 0x20 | 0x0F,
		.base_u = 0x00,
	};

	/* User data segment */
	gdt[4] = (struct gdt_e)
	{
		.limit_l = 0xFFFF,
		.base_l = 0x0000,
		.base_m = 0x00,
		.access_byte = 0xF2,	/* 0b11110010 */
		.flags_limit = 0x00 | 0x0F,
		.base_u = 0x00,
	};

	/* Tast State Segment */
	gdt[5] = (struct gdt_e)
	{
		.limit_l = sizeof (tss) - 1,
		.base_l = ((__u64) & tss) & 0xFFFF,	/* Least significant 4 bits */
		.base_m = (((__u64) & tss) >> 16) & 0xFF,	/* Next byte */
		.access_byte = 0x80 | TSS_A,
		.flags_limit = 0x00,
		.base_u = (((__u64) & tss) >> 24) & 0xFF,	/* Next byte of base */
	};

	gdt[6] = (struct gdt_e)
	{
		.system_segment.base_ih =
			(((__u64) & tss) >> 32) & 0xFFFFFFFF,
		.system_segment.reserved = 0,
	};
}
