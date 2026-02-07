#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <rks/types.h>

struct tss_t
{
	__u32 reserved0;
	__u64 rsp0; /* Kernel stack pointer */
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
	__u16 limit_l;
	__u16 base_l; /* Lower base address 16 bits */
	__u8  base_m; /* Middle base address byte */

	union
	{
		struct
		{
			__u32 p_bit : 1; /* Present bit */
			__u32 dpl   : 2; /* Descriptor Privilege Level */
			__u32 dt    : 1; /* Descriptor type bit */

			union
			{
				/* Specify a system segment if the descriptor bit isn't set (0) */
				enum
				{
					LDT   = 0x2, /* Local descriptor table */
					TSS_A = 0x9, /* Available Task State Segment */
					TSS_B = 0xB,
				} type : 4; /* AB.ACC = 0 */

				struct
				{
					__u32 exec : 1; /* Executable bit */
					__u32 dc   : 1; /* Direction bit */
					__u32 rw   : 1; /* Readable/Writable bit */
					__u32 acc  : 1; /* Accessed bit */
				};
			};
		};

		__u8 raw;
	} access_byte;

	__u32 limit_u : 4; /* 4 upper bits of limit address */
	struct
	{
		__u32 gra      : 1; /* Granularity flag */
		__u32 db       : 1; /* Size flag */
		__u32 lg       : 1; /* Long-mode code flag */
		__u32 reserved : 1;
	} flags;
	__u8 base_u; /* Upper byte of base address */
};

/* GDT descriptor */
struct gdt_d
{
	void         *gdt_limit;
	struct gdt_e *gdt_start;
};

#endif /* DESCRIPTOR_H */
