#ifndef EARLY_ALLOC_H
#define EARLY_ALLOC_H

#include <rks/types.h>

enum alloc_flags
{
	FILL_ZEROS,
	FILL_ONES,
	FILL_NOTHING,
};

void* _early_alloc ( size_t size, u32 align, enum alloc_flags alloc_flag );

#endif /* EARLY_ALLOC_H */
