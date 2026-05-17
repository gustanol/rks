#ifndef IDT_H
#define IDT_H

#include <rks/types.h>
#include <rks/kernel/mem/vmm.h>

/* IDT entry */
struct __attribute__ ( ( packed ) )
{
	u16 offset_l;
	u16 selector;
	ubyte_t ist;
	ubyte_t type_attributes;
	u16 offset_m;
	u32 offset_u;
	pad_t reserved[2];
} idt_entry;

/* IDT descriptor */
struct __attribute__ ( ( packed ) )
{
	u16 limit;
	vaddr_t base;
} idt_descriptor;

#endif /* IDT_H */
