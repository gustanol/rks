#ifndef _KERNEL_ALLOC_HEAP_H
#define _KERNEL_ALLOC_HEAP_H

#include <rks/kernel/mem/alloc/types.h>
#include <rks/kernel/err/kerr.h>

HEAP* alloc_heap_create ( void );
kerr_t alloc_heap_destroy ( struct heap* heap );
void* alloc_heap_malloc ( struct heap* heap, size_t size );
kerr_t alloc_heap_mfree ( struct heap* heap, void* ptr );
kerr_t alloc_heap_grow ( struct heap* heap, size_t frames_needed );

#endif /*  _KERNEL_ALLOC_HEAP_H */
