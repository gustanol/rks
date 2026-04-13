#ifndef EARLY_ALLOC_H
#define EARLY_ALLOC_H

#include <rks/types.h>
#include <rks/kernel/mem/pmm.h>

#define EARLY_HEAP_SIZE (FRAME_SIZE * 16)

void* early_alloc (size_t size);

#endif /* EARLY_ALLOC_H */
