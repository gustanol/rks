#ifndef _KERNEL_SUBSYS_ALLOC_HEAP_H
#define _KERNEL_SUBSYS_ALLOC_HEAP_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/vmm/types.h>

#define CHUNK_LIMIT_BOUNDARY 64

/* Chunk's size unconsidering metadata */
#define CHUNK_SIZE(SIZE) \
        ( ( SIZE ) - sizeof ( struct chunk_meta ) )

/* Ignores the starting metadata of a chunk */
#define META_SHIFT(ADDR) \
        ( ( char * ) ( ADDR ) + sizeof ( struct chunk_meta ) )

/**
 * As it suggests, this metadata struct describes a chunk of memory
 * It acts like a linked list, referencing the next chunk of the chain
 **/
struct chunk_meta
{
	size_t size;            /* Chunk size */
	int is_free;            /* Is this chunk free? */
	struct chunk_meta* next;        /* Next chunk metadata */
};

/* This backend operation functions */
static kerr_t __heap_init ( struct heap* heap );

/* UPDATE THE ARGUMENTS */
static void* _heap_malloc ( size_t size );
static kerr_t _heap_free ( void* start, size_t size );

#endif /* _KERNEL_SUBSYS_ALLOC_HEAP_H */
