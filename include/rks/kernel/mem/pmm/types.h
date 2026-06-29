#ifndef _KERNEL_PMM_TYPES_H
#define _KERNEL_PMM_TYPES_H

#include <rks/kernel/types.h>

/* Define page size as the same size as physical frames */
#define FRAME_SIZE PAGE_SIZE

/* Physical address type */
typedef addr_t paddr_t;

#endif /* _KERNEL_PMM_TYPES_H */
