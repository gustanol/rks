#ifndef _X86_64_CPU_TSS_H
#define _X86_64_CPU_TSS_H

#include <stddef.h>
#include <stdint.h>

struct tss
{

	uint32_t reserved0;
	uint64_t rsp0;              /* Kernel stack pointer */
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t reserved1;
	uint64_t ist[7];            /* Interrupt Stack Table */
	uint64_t reserved2;
	uint64_t reserved3;
	uint64_t iomap_base;
} __attribute__ ( ( packed ) );

/* External TSS pointer to be used by GDT */
struct tss* g_tss;

#endif /* _X86_64_CPU_TSS_H */
