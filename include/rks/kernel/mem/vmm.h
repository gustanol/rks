#ifndef VMM_H
#define VMM_H

#include <rks/types.h>
#include "pmm.h"

#define PAGE_SIZE 4096

/* Flags for mapping */
typedef enum
{
	VMM_PRESENT = 1 << 0,
	VMM_RW = 1 << 1,
	VMM_USER = 1 << 2,
	VMM_NOCACHE = 1 << 3,
} vmm_map_flags_t;

typedef addr_t vaddr_t;

void vmm_init ( void );

#endif /* VMM_H */
