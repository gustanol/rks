#ifndef _X86_CPU_IDT_H
#define _X86_CPU_IDT_H

#include <stddef.h>
#include <stdint.h>

/* IDT entry */
struct __attribute__ ( ( packed ) )
{
	uint16_t offset_l;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attributes;
	uint16_t offset_m;
	uint32_t offset_u;
	uint32_t reserved;
} idt_entry;

/* IDT descriptor */
struct __attribute__ ( ( packed ) )
{
	uint16_t limit;
	uint64_t base;
} idt_descriptor;

#endif /* _X86_CPU_IDT_H */
