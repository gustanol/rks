#ifndef _X86_64_CPU_GDT_H
#define _X86_64_CPU_GDT_H

#include <stddef.h>
#include <stdint.h>

/* GDT entry */
struct gdt_entry
{
	union
	{
		struct
		{
			uint16_t limit_l;   /* Lower 16-bit limit value */
			uint16_t base_l;    /* Lower base address 16 bits */
			uint8_t base_m;     /* Middle base address byte */

			/*
			 * P (1b) = Present bit (should always be 1)
			 * DPL (2b) = Descriptor Privilege Level
			 *   - 0 = Highest privilege (kernel mode)
			 *   - 3 = Lowest privilege (user mode)
			 * S (1b) = Descriptor type bit
			 *   - 0 = System segment (must use one value from `enum segment_type`
			 *     - Type (4b) = Type of system segment (replace the remaining bits)
			 *   - 1 = Code or data segment
			 * E (1b) = Executable bit
			 *   - 0 = Data segment
			 *   - 1 = Code segment
			 * DC (1b) = Direction bit
			 *   - Data segments
			 *     - 0 = Segment grows up
			 *     - 1 = Segment grows down
			 *   - Code segments
			 *     - 0 = Segment can only be executed the DPL ring
			 *     - 1 = Segment can be executed from the DPL or lower privilege level
			 * RW (1b) = Readable/Writable bit
			 *   - Data segments
			 *     - 0 = Write not allowed
			 *     - 1 = Write is allowed
			 *   - Code segments
			 *     - 0 = Read not allowed
			 *     - 1 = Read is allowed
			 *     - Write never allowed
			 * A (1b) = Accessed bit
			 */
			uint8_t access_byte;

			/*
			 * G (1b) = Granularity flag
			 *   - 0 = Byte granularity
			 *   - 1 = Page granularity
			 * DB (1b) = Size flag
			 *   - 0 = 16-bit protected mode segment
			 *   - 1 = 32-bit protected mode segment
			 *   - Should not be set for a 64-bit long mode GDT
			 * L (1b) = Long-mode code flag
			 *   - 1 = 64-bit code segment
			 * Reserved (1b)
			 * LIMIT (4b) = 4 upper limit bits
			 */
			uint8_t flags_limit;
			uint8_t base_u;
		};

		struct
		{
			uint32_t base_ih;   /* Upper 32 bits base address */
			uint32_t reserved;
		} system_segment;
	};
} __attribute__ ( ( packed ) );

/* GDT descriptor */
struct gdt_descriptor
{
	uint16_t limit;
	uint64_t start;
} __attribute__ ( ( packed ) );

/* Types of semgnets to choose */
enum
{

	/* Local Descriptor Table */
	LDT = 0x2,

	/* Available Task State Segment */
	TSS_A = 0x9,

	/* Busy Task State Segment */
	TSS_B = 0xB,
} segment_type;

#endif /* _X86_64_CPU_GDT_H */
