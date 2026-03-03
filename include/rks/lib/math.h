#ifndef MATH_H
#define MATH_H

#include <rks/types.h>

#define BIN_BASE 2
#define OCT_BASE 8
#define DEC_BASE 10
#define HEX_BASE 16

typedef size_t nlength;

size_t ilog (ul64 num, size_t base);
size_t ilog2 (ul64 num);

static inline size_t
ilog10 (ul64 num)
{
	return ilog (num, 10);
}

static inline nlength
numlen (ul64 num, size_t base)
{
	return ilog (num, base) + 1;
}

#define POW16(n) (1ULL << ((n) * 4))

#endif /* MATH_H */
