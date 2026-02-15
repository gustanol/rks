#ifndef IDT_H
#define IDT_H

#include <rks/types.h>

struct idt_e
{
	__u16 offset_l;
	__u16 selector;
	__u8 ist;
	__u8 type_attributes;
	__u16 offset_m;
	__u32 offset_u;
	__u32 reserved;
};

#endif /* IDT_H */
