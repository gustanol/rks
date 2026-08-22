#ifndef _X86_MEM_EARLY_ALLOC_H
#define _X86_MEM_EARLY_ALLOC_H

#include <stddef.h>
#include <stdint.h>

void* _early_alloc ( size_t size, uint32_t align );

#endif /* _X86_MEM_EARLY_ALLOC_H */
