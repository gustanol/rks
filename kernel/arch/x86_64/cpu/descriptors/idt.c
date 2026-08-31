#include <arch/mem/early_alloc.h>
#include <cpu/descriptors/idt.h>

#define IDT_ENTRIES 256

/* Macro used to load a new IDT */
#define LOAD_IDT(DESCRIPTOR) \
        __asm__ volatile ( \
                "lidt %0" \
                : \
                : "m"(*DESCRIPTOR) \
                : "memory" \
        )

/* Macro used to populate the IDT entries by passing the attributes directly */
#define IDT_ENTRY(OFFSET, SELECTOR, IST, TYPE_ATTRIBUTES) \
	( struct idt_entry ) \
	{ \
		.offset_l = ( ( uint64_t ) ( OFFSET ) ) & 0xFFFF, \
		.selector = ( SELECTOR ), \
		.ist = ( IST ), \
		.type_attributes = ( TYPE_ATTRIBUTES ), \
		.offset_m = ( ( uint64_t ) ( OFFSET ) >> 16 ) & 0xFFFF, \
		.offset_u = ( ( uint64_t ) ( OFFSET ) >> 32 ) & 0xFFFFFFFF, \
		.reserved = { }, \
	}

/* Prototypes */
static void __set_idt_entries ( struct idt_entry* idt );

/* IDT static object */
struct idt_entry idt[IDT_ENTRIES];

/**
 * Set and load a new 64-bit Interrupt Descriptor Table
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `set_64_descriptors`
 **/
void
_setup_idt ( void )
{

	/* Sets just the necessary ISRs */
	__set_idt_entries ( idt );

	/* Set the IDT descriptor */
	struct idt_descriptor desc = {
		.limit = ( IDT_ENTRIES * sizeof ( struct idt_entry ) ) - 1,
		.base = ( uint64_t ) & idt,
	};

	/* Loads it */
	LOAD_IDT ( &desc );
}

/*
 * Set the essential IDT entries
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct idt_entry* idt`: IDT array pointer
 * Callers:
 *   - `_setup_idt`
 */
void
__set_idt_entries ( struct idt_entry* idt )
{

	/* External Interrupt Service Routines */
	extern void isr0 ( void );
	extern void isr1 ( void );
	extern void isr2 ( void );
	extern void isr3 ( void );
	extern void isr4 ( void );
	extern void isr5 ( void );
	extern void isr6 ( void );
	extern void isr7 ( void );
	extern void isr8 ( void );
	extern void isr10 ( void );
	extern void isr11 ( void );
	extern void isr12 ( void );
	extern void isr13 ( void );
	extern void isr14 ( void );

	/* For each entry, pass the memory address of the correspondent ISR */

	idt[0] = IDT_ENTRY ( isr0, 0x08, 0, 0x8E );
	idt[1] = IDT_ENTRY ( isr1, 0x08, 0, 0x8F );
	idt[2] = IDT_ENTRY ( isr2, 0x08, 0, 0x8E );
	idt[3] = IDT_ENTRY ( isr3, 0x08, 0, 0x8F );
	idt[4] = IDT_ENTRY ( isr4, 0x08, 0, 0x8F );
	idt[5] = IDT_ENTRY ( isr5, 0x08, 0, 0x8E );
	idt[6] = IDT_ENTRY ( isr6, 0x08, 0, 0x8E );
	idt[7] = IDT_ENTRY ( isr7, 0x08, 0, 0x8E );
	idt[8] = IDT_ENTRY ( isr8, 0x08, 0, 0x8E );
	idt[10] = IDT_ENTRY ( isr10, 0x08, 0, 0x8E );
	idt[11] = IDT_ENTRY ( isr11, 0x08, 0, 0x8E );
	idt[12] = IDT_ENTRY ( isr12, 0x08, 0, 0x8E );
	idt[13] = IDT_ENTRY ( isr13, 0x08, 0, 0x8E );
	idt[14] = IDT_ENTRY ( isr14, 0x08, 0, 0x8E );
}
