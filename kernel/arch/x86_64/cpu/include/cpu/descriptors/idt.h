#ifndef _X86_64_CPU_IDT_H
#define _X86_64_CPU_IDT_H

#include <stddef.h>
#include <stdint.h>

/* IDT entry */
struct idt_entry
{
	uint16_t offset_l;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attributes;
	uint16_t offset_m;
	uint32_t offset_u;
	uint32_t reserved;
} __attribute__ ( ( packed ) );

/* IDT descriptor */
struct idt_descriptor
{
	uint16_t limit;
	uint64_t base;
} __attribute__ ( ( packed ) );

#endif /* _X86_64_CPU_IDT_H */
