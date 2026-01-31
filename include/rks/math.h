#ifndef MATH_H
#define MATH_H

#include <rks/types.h>

#define BIN_BASE 2
#define OCT_BASE 8
#define DEC_BASE 10
#define HEX_BASE 16

typedef __u32 nlength_t;

size_t ilog (__u64 num, __u32 base);
size_t ilog2 (__u64 num);

static inline __u32
ilog10 (__u32 num)
{
	return ilog (num, 10);
}

static inline nlength_t
nlength (__u32 num, __u32 base)
{
	return ilog (num, base) + 1;
}

#define POW16(n) \
        (1ULL << ((n) * 4))

#endif /* MATH_H */
