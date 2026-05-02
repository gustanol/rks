#ifndef BITWISE_H
#define BITWISE_H

#include <rks/types.h>

int first_bitmap_bit ( char* bitmap, u32 size, int bit );

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

/*
 * VALUE + ALIGN - 1
 *   Add the value of the alignment to get the next upper aligned one
 *   The '- 1' is to avoid getting wrong results from already-aligned values
 * E.G.: ALIGN_UP ( 0x9000, 0x1000 ) = 0xA000 (0x9000 is already aligned)
 * So, 0x9000 + 0xFFF = 0x9FFF
 *
 * ~ ( ALIGN - 1 )
 *   Swipe all previous bits to zero and make an AND operation with the first operation
 * E.G: ~ ( 0x1000 - 1 ) = ~ ( 0xFFF ) = 0xFF000 & 0x9000 = 0x9000
 */
#define ALIGN_UP(VALUE, ALIGN) \
        ( ( ( VALUE ) + ( ALIGN ) - 1 ) & ~( ( ALIGN ) - 1 ) )

/*
 * This is simpler
 *
 * It has the same principle, but with the initial sum
 * We don't need get the next aligned portion, we just need to "clean" the lower bits
 *
 * E.G.: ALIGN_UP ( 0x8579, 0x1000 )
 *   - ( 0x8579 & ~ ( 0x1000 - 1 ) ) = ( 0x8579 & ~ ( 0xFFF ) ) = ( 0x8579 & 0xF000 ) = 0x8000
 */
#define ALIGN_DOWN(VALUE, ALIGN) \
        ( ( VALUE ) & ~( ( ALIGN ) - 1 ) )

/* ======================================================== */

#endif /* BITWISE_H */
