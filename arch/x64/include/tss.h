#ifndef TSS_H
#define TSS_H

#include <rks/types.h>

typedef struct
{
	pad_t reserved0[2];     /* 32 bits */
	addr_t rsp0;            /* Kernel stack pointer */
	addr_t rsp1;
	addr_t rsp2;
	pad_t reserved1[4];     /* 64 bits */
	addr_t ist[7];          /* Interrupt Stack Table */
	pad_t reserved2[4];     /* 64 bits */
	pad_t reserved3[4];     /* 64 bits */
	addr_t iomap_base;
} __attribute__ ( ( packed ) ) tss_t;

extern tss_t tss;

#endif /* TSS_H */
