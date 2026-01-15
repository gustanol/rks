#include <rks/math.h>
#include <rks/string.h>

/* Array mapping each algarism (considering any base) */
static char nmap[17] = "0123456789ABCDEF";

/*
 * Convert a number to ASCII text
 *
 * -> `size_t`: Bytes copied to buffer
 * @value: The number
 * @base: Desire base
 * @buf: Buffer pointer
 * @buf_s: Buffer size
 */
size_t itoa(__u64 value, __u32 base, buf_t *buf, size_t buf_s)
{
	do
	{
		buf[--buf_s]  = nmap[value % base];
		value        /= base;
	} while (value > 0);
	return nlength(value, base);
}
