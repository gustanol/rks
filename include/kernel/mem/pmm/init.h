#ifndef _KERNEL_PMM_INIT_H
#define _KERNEL_PMM_INIT_H

#include <kernel/boot/mmap.h>
#include <kernel/kerr/kerr.h>

/* PMM initializer function */
kerr_t pmm_init ( struct mmap_tag* mmap );

#endif /* _KERNEL_PMM_INIT_H */
