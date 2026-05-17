#ifndef TSS_H
#define TSS_H

#include <rks/types.h>
#include <rks/kernel/mem/vmm.h>

struct __attribute__ ( ( packed ) )
{
	pad_t reserved0[2];     /* 32 bits */
	vaddr_t rsp0;            /* Kernel stack pointer */
	vaddr_t rsp1;
	vaddr_t rsp2;
	pad_t reserved1[4];     /* 64 bits */
	vaddr_t ist[7];          /* Interrupt Stack Table */
	pad_t reserved2[4];     /* 64 bits */
	pad_t reserved3[4];     /* 64 bits */
	vaddr_t iomap_base;
} tss;

#endif /* TSS_H */
