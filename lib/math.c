#include <rks/lib/math.h>

/*
 * Integer logarithm operation
 *
 * -> `size_t`: Exponent
 * @num: Number
 * @base: Base
 */
size_t
ilog (ul64 num, size_t base)
{
	/* THe logarithm of zero is zero */
	if (num == 0)
		return num;

	/* Result */
	ul64 res = 0;

	/*
	 * From here, the passed number is divided by the given base multiples times
	 *
	 * It means the last digit is "lost"
	 *
	 * We must stop when the number is less the its base because it expects a values less than "real" one
	 */
	for (; num >= base; res++ /* Each `d * base^{n} */ )
		num /= base;
	return res;
}
