#ifndef BITWISE_H
#define BITWISE_H

#define BITMAP_SET_BIT(bitmap, bit) \
        bitmap[(bit) / 8] |= (1 << ((bit) % 8))

#define BITMAP_CLEAR_BIT(bitmap, bit) \
        bitmap[(bit) / 8] &= ~(1 << ((bit) % 8))

#endif /* BITWISE_H */
