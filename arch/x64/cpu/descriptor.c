#include "descriptor.h"

extern void _setup_descriptors (void);

#define GDT_ENTRIES 7

static struct gdt_e gdt[GDT_ENTRIES];
// static struct tss_t tss;

static inline void __load_gdt (struct gdt_d *descriptor);
static void        __set_gdt_entries (void);

void
_setup_descriptors (void)
{
	/* GDT setup */
	__set_gdt_entries ();

	struct gdt_d gdt_desc = {
		.gdt_limit = (GDT_ENTRIES * sizeof gdt[0]) + gdt,
		.gdt_start = gdt,
	};

	__load_gdt (&gdt_desc);
	// load TSS
}

void
__set_gdt_entries (void)
{
	/* Null entry */
	gdt[0] = (struct gdt_e){ 0 };

	/* Kernel code segment */
	gdt[1] = (struct gdt_e){
		.limit_l           = 0xFFFF,
		.base_l            = 0x0000,
		.base_m            = 0x00,

		.access_byte.p_bit = 1,
		.access_byte.dpl   = 0, /* Ring zero */
		.access_byte.dt    = 1, /* Code or Data segment */
		.access_byte.exec  = 1, /* Code segment */
		.access_byte.dc    = 0, /* Only executable by ring 0 */
		.access_byte.rw    = 1,
		.access_byte.acc   = 1,

		.limit_u           = 0xF,

		.flags.gra         = 1,
		.flags.db          = 0,
		.flags.lg          = 1,

		.base_u            = 0,
	};

	/* Kernel data segment */
	gdt[2] = (struct gdt_e){
		.limit_l           = 0xFFFF,
		.base_l            = 0x0000,
		.base_m            = 0x00,

		.access_byte.p_bit = 1,
		.access_byte.dpl   = 0, /* Ring zero */
		.access_byte.dt    = 1, /* Not a system segment */
		.access_byte.exec  = 0, /* Data segment */
		.access_byte.dc    = 0,
		.access_byte.rw    = 1,
		.access_byte.acc   = 1,

		.limit_u           = 0xF,

		.flags.gra         = 1,
		.flags.db          = 0,
		.flags.lg          = 1,

		.base_u            = 0,
	};

	/* User code segment */
	gdt[3] = (struct gdt_e){
		.limit_l           = 0xFFFF,
		.base_l            = 0x0000,
		.base_m            = 0x00,

		.access_byte.p_bit = 1,
		.access_byte.dpl   = 3, /* Ring three */
		.access_byte.dt    = 1, /* Code or Data segment */
		.access_byte.exec  = 1,
		.access_byte.dc    = 0,
		.access_byte.rw    = 1,
		.access_byte.acc   = 1,

		.limit_u           = 0xF,

		.flags.gra         = 1,
		.flags.db          = 0,
		.flags.lg          = 1,

		.base_u            = 0,
	};

	/* User data segment */
	gdt[4] = (struct gdt_e){
		.limit_l           = 0xFFFF,
		.base_l            = 0x0000,
		.base_m            = 0x00,

		.access_byte.p_bit = 1,
		.access_byte.dpl   = 3, /* Ring three */
		.access_byte.dt    = 1, /* Code or Data segment */
		.access_byte.exec  = 0,
		.access_byte.dc    = 0,
		.access_byte.rw    = 1,
		.access_byte.acc   = 1,

		.limit_u           = 0xF,

		.flags.gra         = 1,
		.flags.db          = 0,
		.flags.lg          = 1,

		.base_u            = 0,
	};

	/* Tast State Segment */
	gdt[5] = (struct gdt_e){
		.limit_l           = 0xFFFF,
		.base_l            = 0x0000,
		.base_m            = 0x00,

		.access_byte.p_bit = 1,
		.access_byte.dpl   = 0,
		.access_byte.dt    = 0,
		.access_byte.type  = TSS_A,

		.limit_u           = 0xF,

		.flags.gra         = 1,
		.flags.db          = 0,
		.flags.lg          = 1,

		.base_u            = 0,
	};
}

void
__load_gdt (struct gdt_d *descriptor)
{
	/* Load the GDT table */
	__asm__ volatile ("lgdt (%0)" : : "r"(descriptor) : "memory");
}
