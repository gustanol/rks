#ifndef _X86_MEM_EARLY_ALLOC_H
#define _X86_MEM_EARLY_ALLOC_H

#include <rks/kernel/types.h>

void* _early_alloc ( size_t size, u32 align );

#endif /* _X86_MEM_EARLY_ALLOC_H */
