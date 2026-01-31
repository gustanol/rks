#include <rks/math.h>
#include <rks/screen/__terminal.h>
#include <rks/string.h>

/* Array mapping each algarism (considering any base) */
static char nmap[17] = "0123456789ABCDEF";
static void __buf_putnumber (int value, size_t base, buf_t ** buf,
                             nlength_t nlen);

/*
 * Convert a number to ASCII text
 *
 * -> `size_t`: Bytes copied to buffer
 * @value: The number
 * @base: Desire base
 * @buf: Buffer pointer
 * @buf_s: Buffer size (non-zero-terminated)
 */
size_t
itoa (__u64 value, __u32 base, buf_t *buf, size_t buf_s)
{
	buf[--buf_s] = '\0';

	do
	    {
		    buf[--buf_s] = nmap[value % base];
		    value /= base;
	    }
	while (value > 0);
	return nlength (value, base);
}

/*
 * Convert an ASCII text to number
 *
 * -> `__u64`: The number
 * @buf: Buffer text
 * @buf_s: Buffer size (number length)
 */
__u64
atoi (buf_t *buf, size_t buf_s)
{
	__u64 res = 0;
	for (size_t i = 0; i < buf_s; i++)
	    {
		    if (buf[buf_s - i - 1] == 'x')
			    break;

		    for (size_t j = 0; j < sizeof (nmap); j++)
			    if (buf[buf_s - i - 1] == nmap[j])
				    res += j * POW16 (i);
	    }
	return res;
}

/*
 * Format a string with arguments and put it into a buffer
 *
 * -> `void`
 * @buf: Buffer pointer to be filled
 * @size: Buffer size
 * @fmt: The string
 * @args: Extra arguments
 */
void
_fmt (buf_t *buf, size_t size, const char *fmt, va_list args)
{
	/* Don't continue if the string was larger than the buffer size */
	if (sizeof (fmt) > size)
		return;

	nlength_t nlen;
	int value;

	do
	    {
		    /* Parse and handle arguments */
		    if (*fmt == '%')
		        {
				fmt++;
				switch (*fmt)
				    {
					    case 'c':  /* Character */
						    {
							    char c = (char)
							            va_arg
							            (args,
							             int);
							    *buf++ = c;
							    break;
						    }

					    case 'b':  /* Binary */
						    {
							    value = (int)
							            va_arg
							            (args,
							             int);
							    nlen = nlength
							            (value,
							             BIN_BASE);

							    __buf_putnumber
							            (value,
							             BIN_BASE,
							             &buf,
							             nlen);
							    break;
						    }

					    case 'o':  /* Octal */
						    {
							    value = (int)
							            va_arg
							            (args,
							             int);
							    nlen = nlength
							            (value,
							             OCT_BASE);
							    __buf_putnumber
							            (value,
							             OCT_BASE,
							             &buf,
							             nlen);
							    break;
						    }

					    case 'd':  /* Decimal */
						    {
							    value = (int)
							            va_arg
							            (args,
							             int);
							    nlen = nlength
							            (value,
							             DEC_BASE);
							    __buf_putnumber
							            (value,
							             DEC_BASE,
							             &buf,
							             nlen);
							    break;
						    }

					    case 'x':  /* Hexadecimal */
						    {
							    value = (int)
							            va_arg
							            (args,
							             int);
							    nlen = nlength
							            (value,
							             HEX_BASE);
							    __buf_putnumber
							            (value,
							             HEX_BASE,
							             &buf,
							             nlen);
							    break;
						    }

					    case 's':  /* String */
						    {
							    const char *str =
							            (const
							             char *)
							            va_arg
							            (args,
							             long);
							    for (;
							         *str != '\0';
							         str++, buf++)
								    *buf = *str;
							    break;
						    }

					    case '%':  /* Literal % */
						    {
							    *buf++ = *fmt;
							    break;
						    }

					    default:
						    {
							    *buf++ = (const
							              char)
							            '%';
							    *buf++ = *fmt;
							    break;
						    }
				    }

				fmt++;
				continue;
			}

		    /* Not a % */
		    *buf++ = *fmt++;
	    }
	while (*fmt);
	*buf = '\0';
}

/*
 * Helper used to add a string-converted number to a buffer
 *
 * -> `void`
 * @value: The number
 * @base: Number base
 * @buf: Buffer double pointer
 * @nlen: Number length
 */
static void
__buf_putnumber (int value, size_t base, buf_t **buf, nlength_t nlen)
{
	buf_t buf_n[nlen + 3];

	/* Format number adding '0_' at start */
	buf_n[0] = '0';
	buf_n[1] = base == BIN_BASE ? 'b'
	        : base == OCT_BASE ? 'o'
	        : base == DEC_BASE ? 'd' : base == HEX_BASE ? 'x' : '?';

	/* Convert the value to string and copy it to given buffer */
	itoa (value, base, buf_n, nlen + 3);
	for (__u32 i = 0; buf_n[i] != '\0'; i++, (*buf)++)
		**buf = buf_n[i];
}
