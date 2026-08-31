#include <arch/mem/early_alloc.h>
#include <cpu/descriptors/gdt.h>
#include <cpu/descriptors/tss.h>

#define GDT_ENTRIES 9

/* Macro used to load a new GDT */
#define LOAD_GDT(DESCRIPTOR) \
	__asm__ volatile ( \
			"lgdt %0" \
			: \
			: "m"(*DESCRIPTOR) \
			: "memory" \
	)

/* Helper macro used to populate a common GDT segment */
#define GDT_ENTRY(LIMIT_LOWER, BASE, ACCESS_BYTE, FLAGS_LIMIT) \
	( struct gdt_entry ) \
	{ \
		.limit_l = ( u16 ) ( LIMIT_LOWER ), \
		.base_l = ( u32 ) ( BASE ) & 0xFFFF, \
		.base_m = ( ( u32 ) ( BASE ) >> 16 ) & 0xFF, \
		.access_byte = ( ubyte_t ) ( ACCESS_BYTE ), \
		.flags_limit = ( ubyte_t ) ( FLAGS_LIMIT ), \
		.base_u = ( ( u32 ) ( BASE ) >> 24 ) & 0xFF, \
	}

/**
 * Helper macro used to populate a system segment
 * NOTE: The previous common segment must had been defined first
 **/
#define GDT_SYSTEM_ENTRY(SEGMENT) \
	(gdt_entry_t) { \
		.system_segment = { \
			.base_ih = ( ( vaddr_t ) ( SEGMENT ) >> 32 ) & 0xFFFFFFFF, \
			.reserved = { 0 }, \
		} \
	}

/* Prototypes */
static void __set_gdt_entries ( struct gdt_entry* gdt );

/**
 * Set a 64-bit Global Descriptor Table
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `set_64_descriptor`
 **/
void
_setup_long_gdt ( void )
{

	/* GDT allocation */
	struct gdt_entry* gdt =
	    ( struct gdt_entry * ) early_alloc ( sizeof ( gdt_entry ) * GDT_ENTRIES );

	/* GDT descriptor */
	struct gdt_descriptor* desc =
	    ( struct gdt_descriptor * ) early_alloc ( sizeof ( gdt_descriptor ) );

	/* Setups all needed GDT entries */
	_set_gdt_entries ( gdt );

	/* *INDENT-OFF* */

	/* Fills the GDT descriptor */
	desc = ( struct gdt_descriptor )
	{
		.limit = ( GDT_ENTRIES * sizeof ( struct gdt_entry ) ) - 1,
		.start = ( vaddr_t ) & gdt,
	};

	/* *INDENT-ON* */

	/* Effectively load it */
	LOAD_GDT ( &gdt_descriptor );
}

/**
 * Set all necessary GDT entries
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct gdt_entry* gdt`: GDT pointer
 * Callers
 *   - `_setup_long_gdt`
 **/
void
__set_gdt_entries ( struct gdt_entry* gdt )
{

	/* *INDENT-OFF* */

	/* Null entry */
	gdt[0] = ( struct gdt_entry )
	{ 0 };

	/* *INDENT-ON* */

	/**
	 * 32-bit kernel code segment (0x8)
	 *
	 * Access byte: 0b10011010
	 * Flags/limit: 0x11001111
	 **/
	gdt[1] = GDT_ENTRY ( 0xFFFF, 0x0, 0x9A, 0xC0 | 0xF );

	/**
	 * 32-bit kernel data segment (0x10)
	 *
	 * Access byte: 0b10010010
	 * Flags/limit: 0x11001111
	 **/
	gdt[2] = GDT_ENTRY ( 0xFFFF, 0x0, 0x92, 0xC0 | 0xF );

	/**
	 * 64-bit kernel code segment (0x18)
	 *
	 * Access byte: 0b10011010
	 * Flags/limit: 0b10101111
	 **/
	gdt[3] = GDT_ENTRY ( 0xFFFF, 0x0, 0x9A, 0xA0 | 0xF );

	/**
	 * 64-bit kernel data segment (0x20)
	 *
	 * Access byte: 0b10010010
	 * Flags/limit: 0b11001111
	 **/
	gdt[4] = GDT_ENTRY ( 0xFFFF, 0x0, 0x92, 0xC0 | 0xF );

	/**
	 * User code segment (0x28)
	 *
	 * Access byte: 0b11111010
	 * Flags/limit: 0b10101111
	 **/
	gdt[5] = GDT_ENTRY ( 0xFFFF, 0x0, 0xFA, 0xA0 | 0xF );

	/**
	 * User data segment (0x30)
	 *
	 * Access byte: 0b11110010
	 * Flags/limit: 0b11001111
	 **/
	gdt[6] = GDT_ENTRY ( 0xFFFF, 0x0, 0xF2, 0xC0 | 0xF );

	/**
	 * Tast State Segment (0x38)
	 *
	 * Access byte: 0b10000000
	 * Flags/limit: 0b00000000
	 **/
	gdt[7] = GDT_ENTRY ( sizeof ( tss ) - 1, ( vaddr_t ) & tss, 0x80 | TSS_A, 0x00 );

	/**
	 * 32 upper bits of TSS virtual address (0x40)
	 **/
	gdt[8] = GDT_SYSTEM_ENTRY ( ( vaddr_t ) & tss );
}
