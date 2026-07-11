#ifndef _KERNEL_SUBSYS_ALLOC_HEAP_H
#define _KERNEL_SUBSYS_ALLOC_HEAP_H

#include <rks/kernel/err/kerr.h>
#include <rks/kernel/mem/vmm/types.h>
#include <rks/kernel/lib/bitwise.h>

#define CHUNK_LIMIT_BOUNDARY 64
#define CHUNK_ALIGN 0x10

/**
 * Calculates the whole chunk size
 *
 * By the whole size, align it up to the macro value
 * `PAYLOAD_SIZE` will consider its padding
 **/
#define CHUNK_SIZE(SIZE) \
        ( ALIGN_UP ( ( SIZE ) + sizeof ( struct chunk_header ) + sizeof ( struct chunk_footer ) ), ( CHUNK_ALIGN ) )

/* Payload chunk's size (unconsidering metadata) */
#define PAYLOAD_SIZE(SIZE) \
        ( ( SIZE ) - sizeof ( struct chunk_header ) - sizeof ( struct chunk_footer ) )

/* Ignores the starting metadata of a chunk */
#define META_SHIFT(ADDR) \
        ( ( char * ) ( ADDR ) + sizeof ( struct chunk_header ) )

/**
 * As it suggests, this metadata struct describes a chunk of memory
 * It acts like a linked list, referencing the next chunk of the chain
 **/
struct chunk_header
{
	size_t size;            /* Chunk size */
	bool is_free;           /* Is this chunk free? */
	bool prev_free;
	struct chunk_header* next;      /* Next chunk metadata */
};

/* Footer metadata for free chunks */
struct chunk_footer
{
	size_t size;
};

/* This backend operation functions */
static kerr_t __heap_init ( struct heap* heap );

/* UPDATE THE ARGUMENTS */
static void* _heap_malloc ( size_t size );
static kerr_t _heap_free ( void* ptr );

#endif /* _KERNEL_SUBSYS_ALLOC_HEAP_H */
