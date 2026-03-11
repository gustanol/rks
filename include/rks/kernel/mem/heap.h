#ifndef HEAP_H
#define HEAP_H

#include <rks/types.h>

#define HEAP_INITIAL_SIZE (1024 * 1024)

void heap_init (void);
void* kmalloc (size_t size);
void kfree (void* ptr);

/* Heap object context header */
struct heap_header
{
	u32 size;		/* Block size (without header) */
	bool is_free;
	struct heap_header* next;
};

#endif /* HEAP_H */
