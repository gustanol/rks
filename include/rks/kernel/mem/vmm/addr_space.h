#ifndef _KERNEL_VMM_ADDR_SPACE_H
#define _KERNEL_VMM_ADDR_SPACE_H

#include "types.h"

/* Address space operations */
ADDR_SPACE* vmm_create_space ( void );
void vmm_destroy_space ( ADDR_SPACE * space );
void vmm_switch_to_space ( ADDR_SPACE * space );

#endif /* _KERNEL_VMM_ADDR_SPACE_H */
