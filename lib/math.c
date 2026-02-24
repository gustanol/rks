#include <rks/lib/math.h>
#include <rks/types.h>

/*
 * Integer logarithm operation
 *
 * -> `size_t`: Exponent
 * @num: Number
 * @base: Base
 */
size_t
ilog (__u64 num, __u32 base)
{
	if (num == 0)
		return num;

	__u32 res = 0;
	for (; num >= base; res++)
		num /= base;
	return res;
}
