#include <rks/lib/bitwise.h>

/*
 * Find the first `bit` bit in a bitsize
 * Helper. There`s macros for specific operations
 * See them in `include/rks/lib/bitwise.h`
 *
 * -> int: Returned bit. -1 if it was not found
 * @bitmap: Base pointer to the bitmap
 * @size: Bitmap size in bytes. Multiply by 8 if you have the size only in bits
 * @bit: First bit to be found: 1 or 0
 */
int
first_bitmap_bit (char* bitmap, u32 size, int bit)
{
	char byte;

	/* Iterate for each byte in bitmap */
	for (u32 i = 0; i < size; i++)
	{
		/*
		 * In here, the formed byte depends of the passed must-find-bit
		 *
		 * To find the first 0 bit, we must use a mask (~) to make an AND operation with a byte
		 * That will turn 0 bits into 1, allowing us to making an AND, determining this byte HAS a zero
		 * e.g.: 1110 1111 -> 0001 0000 & 1111 1111 != 0; It has a zero
		 *
		 * To find the first 1 bit, we must not mask because 1 bits can be found with AND
		 * e.g.: 0010 0000 & 1111 1111 ~= 0; It has a one
		 */
		byte = (bit) ? bitmap[i] : ~bitmap[i];
		if (byte & 0xFF)
		{
			/* Iterate for each bit in this byte */
			for (u32 j = 0; j < 8; j++)
				/*
				 * For each iteration, we verify if the bit `j` is the one we`re looking for
				 *
				 * For this, we right shift 1 `j` times, creating a mask
				 * Then, it happens an AND operation to verify that bit
				 *
				 * e.g.:
				 * - j = 2:
				 *   1 << 2 = 100
				 * - j = 5:
				 *   1 << 5 = 100000
				 */
				if (byte & (1 << j))
					/*
					 * i * 8: The bit (byte * 8)
					 * j: The bit within the byte
					 */
					return (i * 8) + j;
		}
	}

	/* If it were not found, return -1 */
	return -1;
}
