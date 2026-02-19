#include <idt.h>

#define IDT_ENTRIES 256
#define LOAD_IDT(descriptor) \
        __asm__ volatile ( \
                "lidt %0" \
                : \
                : "m"(*descriptor) \
                : "memory" \
        )

#define IDT_ENTRY(OFFSET, SELECTOR, IST, TYPE_ATTRIBUTES) \
 (struct idt_e) \
 { \
        .offset_l = ((__u64)(OFFSET)) & 0xFFFF, \
        .selector = SELECTOR, \
        .ist = IST, \
        .type_attributes = TYPE_ATTRIBUTES, \
        .offset_m = ((__u64)(OFFSET)) >> 16 & 0xFFFF, \
        .offset_u = ((__u64)(OFFSET)) >> 32 & 0xFFFFFFFF, \
        .reserved = 0, \
 }

static struct idt_e idt[IDT_ENTRIES] __attribute__ ((aligned (16)));	/* IDT struct */
static struct idtr idtr __attribute__ ((aligned (16)));	/* IDT register */

static void __set_idt_entries (void);

void
_setup_idt (void)
{
	__set_idt_entries ();
	idtr.limit = (IDT_ENTRIES * sizeof (struct idt_e)) - 1;
	idtr.base = (__u64) & idt;

	LOAD_IDT (&idtr);
}

void
__set_idt_entries (void)
{
	/* Extern ISRs */
	extern void isr0 (void);
	extern void isr1 (void);
	extern void isr2 (void);
	extern void isr3 (void);
	extern void isr4 (void);
	extern void isr5 (void);
	extern void isr6 (void);
	extern void isr7 (void);
	extern void isr8 (void);
	extern void isr10 (void);
	extern void isr11 (void);
	extern void isr12 (void);
	extern void isr13 (void);
	extern void isr14 (void);

	/* Set entries */
	idt[0] = IDT_ENTRY (isr0, 0x08, 0, 0x8E);
	idt[1] = IDT_ENTRY (isr1, 0x08, 0, 0x8F);
	idt[2] = IDT_ENTRY (isr2, 0x08, 0, 0x8E);
	idt[3] = IDT_ENTRY (isr3, 0x08, 0, 0x8F);
	idt[4] = IDT_ENTRY (isr4, 0x08, 0, 0x8F);
	idt[5] = IDT_ENTRY (isr5, 0x08, 0, 0x8E);
	idt[6] = IDT_ENTRY (isr6, 0x08, 0, 0x8E);
	idt[7] = IDT_ENTRY (isr7, 0x08, 0, 0x8E);
	idt[8] = IDT_ENTRY (isr8, 0x08, 0, 0x8E);
	idt[10] = IDT_ENTRY (isr10, 0x08, 0, 0x8E);
	idt[11] = IDT_ENTRY (isr11, 0x08, 0, 0x8E);
	idt[12] = IDT_ENTRY (isr12, 0x08, 0, 0x8E);
	idt[13] = IDT_ENTRY (isr13, 0x08, 0, 0x8E);
	idt[14] = IDT_ENTRY (isr14, 0x08, 0, 0x8E);
}
