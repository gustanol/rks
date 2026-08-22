#ifndef _KERNEL_ALLOC_DISPATCH_H
#define _KERNEL_ALLOC_DISPATCH_H

#include <kernel/kerr/kerr.h>

/* Heap dispatchers */
struct heap* _alloc_dispatch_heap_create ( void );
kerr_t _alloc_dispatch_heap_destroy ( struct heap* heap );
void* _alloc_dispatch_heap_malloc ( struct heap* heap, size_t size );
kerr_t _alloc_dispatch_heap_mfree ( struct heap* heap, void* ptr );
kerr_t _alloc_dispatch_heap_grow ( struct heap* heap, size_t frames_needed );

/* Kernel wrappers */
void* _alloc_dispatch_k_malloc ( size_t size );
kerr_t _alloc_dispatch_k_mfree ( void* ptr );

#endif /* _KERNEL_ALLOC_DISPATCH_H */
