#include <gdt.h>
#include <tss.h>

/* Just the necessary */
#define GDT_ENTRIES 7

#define LOAD_GDT(DESCRIPTOR) \
        __asm__ volatile ( \
                "lgdt %0" \
                : \
                : "m"(*DESCRIPTOR) \
                : "memory" \
        )

/* Populate a GDT entry */
#define GDT_ENTRY(LIMIT_LOWER, BASE, ACCESS_BYTE, FLAGS_LIMIT) \
        ( gdt_entry_t ) \
        { \
                .limit_l = ( u16 ) ( LIMIT_LOWER ), \
                .base_l = ( u32 ) ( BASE ) & 0xFFFF, \
                .base_m = ( ( u32 ) ( BASE ) >> 16 ) & 0xFF, \
                .access_byte = ( ubyte_t ) ( ACCESS_BYTE ), \
                .flags_limit = ( ubyte_t ) ( FLAGS_LIMIT ), \
                .base_u = ( ( u32 ) ( BASE ) >> 24 ) & 0xFF, \
        }

/*
 * Macro used to set the 32 upper bits for a segment (such as TSS)
 *
 * The previous GDT entry must describe this segment and set the lower 32 base bits
 */
#define GDT_SEGMENT_ENTRY(SEGMENT) \
    (gdt_entry_t) { \
        .system_segment = { \
            .base_ih = ( ( addr_t ) ( SEGMENT ) >> 32 ) & 0xFFFFFFFF, \
            .reserved = { 0 }, \
        } \
    }

/* GDT struct */
static gdt_entry_t gdt[GDT_ENTRIES] __attribute__ ( ( aligned ( 16 ) ) );
static gdt_desc_t gdt_descriptor __attribute__ ( ( aligned ( 16 ) ) );

/* Prototypes */
static void __set_gdt_entries ( void );

/*
 * Helper to set and load a 64-bit GDT
 *
 * -> void
 */
void
_setup_long_gdt ( void )
{
	/* Setup all needed GDT entries */
	__set_gdt_entries (  );

	/* *INDENT-OFF* */

        /* Fill the GDT descriptor */
	gdt_descriptor = ( gdt_desc_t )
	{
	        .limit = ( GDT_ENTRIES * sizeof ( gdt_entry_t ) ) - 1,
                .start = ( addr_t ) & gdt,
        };
	/* *INDENT-ON* */

	LOAD_GDT ( &gdt_descriptor );
}

/*
 * Set the essential GDT entries
 *
 * -> void
 */
void
__set_gdt_entries ( void )
{
        /* *INDENT-OFF* */

	/* Null entry */
	gdt[0] = ( gdt_entry_t )
	{ 0 };

        /* *INDENT-ON* */

	/* Kernel code segment */
	gdt[1] = GDT_ENTRY ( 0xFFFF, 0, 0x9A, 0x20 | 0x0F );

	/* Kernel data segment */
	gdt[2] = GDT_ENTRY ( 0xFFFF, 0, 0x92, 0x00 | 0x0F );

	/* User code segment */
	gdt[3] = GDT_ENTRY ( 0xFFFF, 0, 0xFA, 0x20 | 0x0F );

	/* User data segment */
	gdt[4] = GDT_ENTRY ( 0xFFFF, 0, 0xF2, 0x00 | 0x0F );

	/* Tast State Segment */
	gdt[5] = GDT_ENTRY ( sizeof ( tss ) - 1, ( addr_t ) & tss, 0x80 | TSS_A,
	                     0x00 );

	/*
	 * For system segments, we must set its base address
	 *
	 * As an address has 64 bits and an entry base has 32 bits, we must use more than one entry
	 *
	 * The first 32 bits are used as the upper 32 bits of TSS base address
	 * The remaining are reserved
	 */
	gdt[6] = GDT_SEGMENT_ENTRY ( &tss );
}
