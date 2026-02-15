#include <idt.h>

#define IDT_ENTRIES 256
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

struct idt_e idt[IDT_ENTRIES];
static void __set_idt_entries (void);

void
_setup_idt (void)
{
	__set_idt_entries ();
}

void
__set_idt_entries (void)
{
	/* Externs ISRs */
	extern char isr0[];
	extern char isr1[];
	extern char isr2[];
	extern char isr3[];
	extern char isr4[];
	extern char isr5[];
	extern char isr6[];
	extern char isr7[];
	extern char isr8[];
	extern char isr10[];
	extern char isr11[];
	extern char isr12[];
	extern char isr13[];
	extern char isr14[];

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
