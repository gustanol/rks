#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <rks/types.h>

struct tss_t
{
	__u32 reserved0;
	__u64 rsp0;		/* Kernel stack pointer */
	__u64 rsp1;
	__u64 rsp2;
	__u64 reserved1;
	__u64 ist[7];
	__u64 reserved2;
	__u64 reserved3;
	__u64 iomap_base;
} __attribute__ ((packed));

/* GDT entry */
struct gdt_e
{
	union
	{
		struct
		{
			__u16 limit_l;
			__u16 base_l;	/* Lower base address 16 bits */
			__u8 base_m;	/* Middle base address byte */

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
			__u8 access_byte;

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
			 */
			__u8 flags_limit;
			__u8 base_u;
		};

		struct
		{
			__u32 base_ih;	/* Upper 32 bits base address */
			__u32 reserved;
		} system_segment;
	};
} __attribute__ ((packed));

/* GDT descriptor */
struct gdt_d
{
	__u16 limit;
	__u64 start;
} __attribute__ ((packed));

enum
{
	LDT = 0x2,		/* Local descriptor table */
	TSS_A = 0x9,		/* Available Task State Segment */
	TSS_B = 0xB,
} segment_type;			/* AB.ACC = 0 */

#endif /* DESCRIPTOR_H */
