#ifndef _KERNEL_VMM_ADDR_SPACE_H
#define _KERNEL_VMM_ADDR_SPACE_H

#include "types.h"
#include <kernel/kerr/kerr.h>

/* Address space operations */
ADDR_SPACE* vmm_create_space ( void );
kerr_t vmm_destroy_space ( ADDR_SPACE * space );
kerr_t vmm_switch_to_space ( ADDR_SPACE * space );

#endif /* _KERNEL_VMM_ADDR_SPACE_H */
