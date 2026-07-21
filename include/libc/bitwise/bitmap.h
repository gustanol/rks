#ifndef _LIBC_BITWISE_BITMAP_H
#define _LIBC_BITWISE_BITMAP_H

#include <stddef.h>
#include <stdint.h>

/* Operations */
int64_t bitmap_find_first_set ( const uint64_t * bitmap, size_t size );
int64_t bitmap_find_first_zero ( const uint64_t * bitmap, size_t size );

/**
 * Set the bit `bit` within a bitmap
 * Libc-base inline function
 *
 * Return -> `void`
 * Arguments:
 *   1. `const char* bitmap`: First bitmap byte portion
 *   2. `uint64_t bit`: Bit index to be set
 * Callers:
 *   - Many
 **/
static inline void
bitmap_set_bit ( const char* bitmap, uint64_t bit )
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
 *   1. `const char* bitmap`: First bitmap byte portion
 *   2. `uint64_t bit`: Bit index to be set
 * Callers:
 *   - Many
 **/
static inline void
bitmap_clear_bit ( const char* bitmap, uint64_t bit )
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

#endif /* _LIBC_BITWISE_BITMAP_H */
