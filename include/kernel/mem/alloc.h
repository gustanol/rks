#ifndef _KERNEL_ALLOC_H
#define _KERNEL_ALLOC_H

#include <rks/kernel/mem/alloc/types.h>
#include <rks/kernel/err/kerr.h>

void* k_malloc ( size_t size );
void* k_calloc ( size_t size );
kerr_t k_mfree ( void* ptr );

#endif /* _KERNEL_ALLOC_H */
