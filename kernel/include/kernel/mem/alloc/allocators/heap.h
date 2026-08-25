#ifndef _KERNEL_ALLOC_HEAP_H
#define _KERNEL_ALLOC_HEAP_H

#include <kernel/mem/alloc/types.h>
#include <kernel/kerr/kerr.h>

HEAP* alloc_heap_create ( void );
kerr_t alloc_heap_destroy ( HEAP * heap );
void* alloc_heap_malloc ( HEAP * heap, size_t size );
kerr_t alloc_heap_mfree ( HEAP * heap, void* ptr );
kerr_t alloc_heap_grow ( HEAP * heap, size_t frames_needed );

#endif /*  _KERNEL_ALLOC_HEAP_H */
