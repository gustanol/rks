#ifndef BITWISE_H
#define BITWISE_H

#include <rks/types.h>

int first_bitmap_bit (char* bitmap, u32 size, int bit);

/* Bitmap operations */
/* ========================================================= */
#define FIND_FIRST_ZERO_BIT(BITMAP, SIZE) \
        first_bitmap_bit ( ( BITMAP ), ( SIZE ), 0)

#define FIND_FIRST_SET_BIT(BITMAP, SIZE) \
        first_bitmap_bit ( ( BITMAP ), ( SIZE ), 1)

#define BITMAP_SET_BIT(BITMAP, BIT) \
        ( BITMAP )[( BIT ) / 8] |= ( 1ULL << ( ( BIT ) % 8 ) )

#define BITMAP_CLEAR_BIT(BITMAP, BIT) \
        ( BITMAP )[( BIT ) / 8] &= ~( 1ULL << ( ( BIT ) % 8 ) )
/* ========================================================= */

/* Alignment */
/* ======================================================== */
#define ALIGN_UP(VALUE, ALIGN) \
        ( ( ( VALUE ) - ( ALIGN ) - 1 ) & ~( ( ALIGN ) - 1 ) )

#define ALIGN_DOWN(VALUE, ALIGN) \
        ( ( VALUE ) & ~( ( ALIGN ) - 1 ) )
/* ======================================================== */

#endif /* BITWISE_H */
