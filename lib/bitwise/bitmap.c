#include <libc/bitwise/bitmap.h>

/**
 * Find the first set bit (`1`) within a bitmap
 * Libc-based function
 *
 * Return -> `int64_t`: Found bit index or -1 if not found
 * Arguments:
 *   1. `const uint64_t* bitmap`: Pointer to the first 64-bit bitmap portion
 *   2. `size_t size`: Bitmap size in bytes
 * Callers:
 *   - Many
 **/
int64_t
bitmap_find_first_set ( const uint64_t* bitmap, size_t size )
{

	/**
         * Convert the size in bytes to 64-bit portions
         *
         * Guarantee it's rounded up
         **/
	size_t num_portions = ( size + 7 ) / 8;

	/* Iterate for each portion */
	for ( size_t portion_idx = 0; portion_idx < num_portions;
	      portion_idx++ )
	{

		/* Current portion */
		uint64_t portion = bitmap[portion_idx];

		/* If the portion isn't zero, the first set bit is in here */
		if ( portion != 0 )

			/**
                         * `portion_idx * 64`: Current portion start
                         * `__builtin_ctzll ( portion )`: Count of trailing zeros
                         *   Ignores all leading zeros and return the first set bit
                         **/
			return ( int64_t ) ( portion_idx * 64 +
		                             __builtin_ctzll ( portion ) );
	}

	/* If it wasn't found, the bitmap is full */
	return -1;
}

/**
 * Find the first zero bit (`0`) within a bitmap
 * Libc-based function
 *
 * Return -> `int64_t`: Found bit index or -1 if not found
 * Arguments:
 *   1. `const uint64_t* bitmap`: Pointer to the first 64-bit bitmap portion
 *   2. `size_t size`: Bitmap size in bytes
 * Callers:
 *   - Many
 **/
int64_t
bitmap_find_first_zero ( const uint64_t* bitmap, size_t size )
{

	/* Number of 64-bit portions from byte size */
	size_t num_portions = ( size + 7 ) / 8;

	/* Iterate for each of them */
	for ( size_t portion_idx = 0; portion_idx < num_portions;
	      portion_idx++ )
	{

		/* Current portion */
		uint64_t portion = bitmap[portion_idx];

		/**
                 * Note that portion can't be completely filled with bits 1 (UINT64_MAX)
                 * For any other cases, there's at least one zero bit in here
                 **/
		if ( portion != UINT64_MAX )

			/**
                         * `portion_idx * 64`: Current portion start
                         * `__builtin_ctzll ( ~portion )`: Count of trailing zeros
                         *   Inverts all bits 1 and do the same operation by
                         *   Ignoring all leading zeros
                         **/
			return ( int64_t ) ( portion_idx * 64 +
		                             __builtin_ctzll ( ~portion ) );
	}

	/* If it wasn't found, again, the bitmap is full */
	return -1;
}
