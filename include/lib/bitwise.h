#ifndef _LIBC_BITWISE_BITMAP_H
#define _LIBC_BITWISE_BITMAP_H

#include <stddef.h>
#include <stdint.h>

#define BIT_TO_BYTE_SIZE(b) \
	( ( ( b ) + 7 ) / 8 )

int64_t bitmap_find_first_set ( const uint64_t * bitmap, size_t size );
int64_t bitmap_find_first_zero ( const uint64_t * bitmap, size_t size );

/**
 * Set the bit `bit` within a bitmap
 * Libc-base inline function
 *
 * Return -> `void`
 * Arguments:
 *   1. `char* bitmap`: First bitmap byte portion
 *   2. `uint64_t bit`: Bit index to be set
 **/
static inline void
bitmap_set_bit ( char* bitmap, uint64_t bit )
{

	/**
	 * The bitmap is divided into byte-size portions
	 *
	 * For that reason, we can get the portion `bit` is just dividing it by 8 (1 byte)
	 *   For example:
	 *   . `bit 12` -> 12 / 8 = 1: Portion 1
	 *   . `bit 65` -> 65 / 8 = 8: Portion 8
	 *
	 * `bit % 8` is the shift `bit` is within a byte
	 *   For example:
	 *   . `bit 15` -> 7
	 *   . `bit 22` -> 6
	 *
	 * With that index, we can access this bit within the portion
	 **/
	bitmap[bit / 8] |= 1ULL << ( bit % 8 );
}

/**
 * Clear the bit `bit` within a bitmap
 * Libc-base inline function
 *
 * Return -> `void`
 * Arguments:
 *   1. `char* bitmap`: First bitmap byte portion
 *   2. `uint64_t bit`: Bit index to be set
 **/
static inline void
bitmap_clear_bit ( char* bitmap, uint64_t bit )
{

	/**
	 * The logic is almost the same as `bitmap_set_bit`
	 *
	 * `1ULL << ( bit % 8 )` are inverted (~),
	 * Which means the aimed bit becomes 0, while all others becomes 1
	 * Making an AND operation results in: bit & 0 = 0
	 **/
	bitmap[bit / 8] &= ~( 1ULL << ( bit % 8 ) );
}

/*
 * 1. `VALUE + ALIGN - 1`
 *   - Add the value of the alignment to get the next upper aligned one
 *   - The '- 1' is to avoid getting wrong results from already-aligned values
 *	   - e.g.: ALIGN_UP ( 0x9000, 0x1000 ) = 0xA000 (0x9000 is already aligned)
 *     - So, 0x9000 + 0xFFF = 0x9FFF
 *
 * 2. `~( ALIGN - 1 )`
 *   - Swipe all previous bits to zero and make an AND operation with the first operation
 *     - e.g.: ~( 0x1000 - 1 ) = ~( 0xFFF ) = 0xFF000 & 0x9000 = 0x9000
 */
#define ALIGN_UP(VALUE, ALIGN) \
        ( ( ( VALUE ) + ( ALIGN ) - 1 ) & ~( ( ALIGN ) - 1 ) )

/*
 * This is simpler
 *
 * It has the same principle, but with the initial sum
 * We don't need get the next aligned portion, we just need to "clean" the lower bits
 *
 * e.g.: ALIGN_UP ( 0x8579, 0x1000 )
 *   - ( 0x8579 & ~( 0x1000 - 1 ) ) = ( 0x8579 & ~( 0xFFF ) ) = ( 0x8579 & 0xF000 ) = 0x8000
 */
#define ALIGN_DOWN(VALUE, ALIGN) \
        ( ( VALUE ) & ~( ( ALIGN ) - 1 ) )

#endif /* _LIBC_BITWISE_BITMAP_H */
