#ifndef BITWISE_H
#define BITWISE_H

#include <rks/types.h>

int first_bitmap_bit (char* bitmap, u32 size, int bit);

#define FIND_FIRST_ZERO_BIT(bitmap, size) \
        first_bitmap_bit(bitmap, size, 0)

#define FIND_FIRST_SET_BIT(bitmap, size) \
        first_bitmap_bit(bitmap, size, 1)

#define BITMAP_SET_BIT(bitmap, bit) \
        bitmap[(bit) / 8] |= (1ULL << ((bit) % 8))

#define BITMAP_CLEAR_BIT(bitmap, bit) \
        bitmap[(bit) / 8] &= ~(1ULL << ((bit) % 8))

#endif /* BITWISE_H */
