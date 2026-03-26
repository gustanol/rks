#ifndef IDT_H
#define IDT_H

#include <rks/types.h>

/* IDT entry */
typedef struct
{
	u16 offset_l;
	u16 selector;
	ubyte_t ist;
	ubyte_t type_attributes;
	u16 offset_m;
	u32 offset_u;
	pad_t reserved[2];
} __attribute__ ( ( packed ) ) idt_entry_t;

/* IDT descriptor */
typedef struct
{
	u16 limit;
	addr_t base;
} __attribute__ ( ( packed ) ) idt_desc_t;

#endif /* IDT_H */
