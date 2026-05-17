#include <cpu/idt.h>
#include <mem/early_alloc.h>

#define IDT_ENTRIES 256

/* Macro used to load a new IDT
 * ============================== */
#define LOAD_IDT(DESCRIPTOR) \
        __asm__ volatile ( \
                "lidt %0" \
                : \
                : "m"(*DESCRIPTOR) \
                : "memory" \
        )

/* Macro used to populate the IDT entries by passing the attributes directly
 * ============================================================== */
#define IDT_ENTRY(OFFSET, SELECTOR, IST, TYPE_ATTRIBUTES) \
 ( struct idt_entry ) \
 { \
        .offset_l = ( ( vaddr_t ) ( OFFSET ) ) & 0xFFFF, \
        .selector = ( SELECTOR ), \
        .ist = ( IST ), \
        .type_attributes = ( TYPE_ATTRIBUTES ), \
        .offset_m = ( ( vaddr_t ) ( OFFSET ) >> 16 ) & 0xFFFF, \
        .offset_u = ( ( vaddr_t ) ( OFFSET ) >> 32 ) & 0xFFFFFFFF, \
        .reserved = { 0 }, \
 }

/* Prototypes */
static void __set_idt_entries ( struct idt_entry* idt );

/*
 * Sets and loads a new 64-bit version Interrupt Descriptor Table
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `set_64_descriptors`
 */
void
_setup_idt ( void )
{
	/* IDT allocation  */
	struct idt_entry* idt =
	        ( struct idt_entry * ) early_alloc ( sizeof ( idt_entry ) *
	                                             IDT_ENTRIES );

	/* IDT descriptor */
	struct idt_descriptor* desc =
	        ( struct idt_descriptor * )
	        early_alloc ( sizeof ( idt_descriptor ) );

	/* Sets just the necessary ISRs */
	__set_idt_entries ( idt );

        /* *INDENT-OFF* */

        /* Set the IDT descriptor */
	idt_descriptor = ( idt_desc_t )
        {
                .limit = ( IDT_ENTRIES * sizeof ( idt_entry_t ) ) - 1,
                .base = ( addr_t ) & idt,
        };
        /* *INDENT-ON* */

	/* Loads it */
	LOAD_IDT ( &idt_descriptor );
}

/*
 * Sets the essential IDT entries
 * Private helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `struct idt_entry* idt`: IDT pointer
 * Callers:
 *   - `_setup_idt`
 */
void
__set_idt_entries ( struct idt_entry* idt )
{
	/* External Interrupt Service Routines
	 * =================================== */
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

	/*
	 * For each entry, pass the memory address of the correspondent ISR
	 */

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
