#include <cpu/gdt.h>
#include <cpu/tss.h>

/* Just the necessary */
#define GDT_ENTRIES 9

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
            .base_ih = ( ( vaddr_t ) ( SEGMENT ) >> 32 ) & 0xFFFFFFFF, \
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
                .start = ( vaddr_t ) & gdt,
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

	/*
	 * 32-bit kernel code segment (0x8)
	 *
	 * Access byte: 0b10011010
	 * Flags/limit: 0x11001111
	 */
	gdt[1] = GDT_ENTRY ( 0xFFFF, 0x0, 0x9A, 0xC0 | 0xF );

	/*
	 * 32-bit kernel data segment (0x10)
	 *
	 * Access byte: 0b10010010
	 * Flags/limit: 0x11001111
	 */
	gdt[2] = GDT_ENTRY ( 0xFFFF, 0x0, 0x92, 0xC0 | 0xF );

	/*
	 * 64-bit kernel code segment (0x18)
	 *
	 * Access byte: 0b10011010
	 * Flags/limit: 0b10101111
	 */
	gdt[3] = GDT_ENTRY ( 0xFFFF, 0x0, 0x9A, 0xA0 | 0xF );

	/*
	 * 64-bit kernel data segment (0x20)
	 *
	 * Access byte: 0b10010010
	 * Flags/limit: 0b11001111
	 */
	gdt[4] = GDT_ENTRY ( 0xFFFF, 0x0, 0x92, 0xC0 | 0xF );

	/*
	 * User code segment (0x28)
	 *
	 * Access byte: 0b11111010
	 * Flags/limit: 0b10101111
	 */
	gdt[5] = GDT_ENTRY ( 0xFFFF, 0x0, 0xFA, 0xA0 | 0xF );

	/*
	 * User data segment (0x30)
	 *
	 * Access byte: 0b11110010
	 * Flags/limit: 0b11001111
	 */
	gdt[6] = GDT_ENTRY ( 0xFFFF, 0x0, 0xF2, 0xC0 | 0xF );

	/*
	 * Tast State Segment (0x38)
	 *
	 * Access byte: 0b10000000
	 * Flags/limit: 0b00000000
	 */
	gdt[7] = GDT_ENTRY ( sizeof ( tss ) - 1, ( vaddr_t ) & tss,
	                     0x80 | TSS_A, 0x00 );

	/*
	 * 32 upper bits of TSS virtual address (0x40)
	 */
	gdt[8] = GDT_SEGMENT_ENTRY ( ( vaddr_t ) & tss );
}
