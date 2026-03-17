#include <rks/lib/math.h>
#include <rks/lib/string.h>

/* Array mapping each digit */
static char nmap[17] = "0123456789ABCDEF";
static void __buf_putnumber ( ul64 value, size_t base, buf_t ** buf,
                              nlength nlen );

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
itoa ( ul64 value, size_t base, buf_t* buf, size_t buf_s )
{
	buf[--buf_s] = '\0';

	do
	{
		/*
		 * For each iteration, the digits will be put at the final to start of buffer
		 *
		 * Dividing a number by any base, we'll get its last digit
		 * Get its modulus from `nmap` (the character corresponding to that value)
		 * - Notice that digits are placed in order in `nmap`; exactly for this
		 *
		 * After getting the last digit, dividing the number by any base will "discard" it
		 */
		buf[--buf_s] = nmap[value % base];
		value /= base;
	} while ( value > 0 );
	return numlen ( value, base );
}

/*
 * Convert an ASCII text to number
 *
 * -> `ul64`: The number
 * @buf: Buffer text
 * @buf_s: Buffer size (number length)
 */
ul64
atoi ( buf_t* buf, size_t buf_s )
{
	ul64 res = 0;

	for ( size_t i = 0; i < buf_s; i++ )
	{
		if ( buf[buf_s - i - 1] == 'x' )
			break;

		for ( size_t j = 0; j < sizeof ( nmap ); j++ )
			if ( buf[buf_s - i - 1] == nmap[j] )
				res += j * POW16 ( i );
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
_fmt ( buf_t* buf, size_t size, const char* fmt, va_list args )
{
	/* Don't continue if the string was larger than the buffer size */
	if ( sizeof ( fmt ) > size )
		return;

	do
	{
		/* Parse and handle arguments */
		if ( *fmt == '%' )
		{
			fmt++;
			switch ( *fmt )
			{
				  /*
				   * For each of the following numeric cases, there's a couple of operations:
				   *
				   * - `value`: The gotten value from args; casted to the correct type
				   * - `nlen`: The amount of digits `value` has; it's used to created a buffer with an exact size
				   * - `__buf_putnumber` call: It calls this function to correctly put the value on buffer
				   */

			  case 'b':    /* Binary */
				  {
					  int value = ( int ) va_arg ( args,
					                               int );
					  nlength nlen = numlen ( value,
					                          BIN_BASE );

					  __buf_putnumber ( value,
					                    BIN_BASE,
					                    &buf, nlen );
					  break;
				  }

			  case 'o':    /* Octal */
				  {
					  int value = ( int ) va_arg ( args,
					                               int );
					  nlength nlen = numlen ( value,
					                          OCT_BASE );

					  __buf_putnumber ( value,
					                    OCT_BASE,
					                    &buf, nlen );
					  break;
				  }

			  case 'd':    /* 32-bit Decimal */
				  {
					  int value = ( int ) va_arg ( args,
					                               int );
					  nlength nlen = numlen ( value,
					                          DEC_BASE );

					  __buf_putnumber ( value,
					                    DEC_BASE,
					                    &buf, nlen );
					  break;
				  }

			  case 'l':    /* Long */
				  {

					  /* Treat "long long" cases */
					  if ( *( fmt + 1 ) == 'l' )
					  {
						  /* We just advanced 1 */
						  fmt++;
						  switch ( *( fmt + 1 ) )
						  {

							    /* Decimal unsigned long long */
						    case 'u':
							    {
								    ul64 value =
								            ( ul64 )
								            va_arg
								            ( args,
								              ul64 );
								    nlength nlen
								            =
								            numlen
								            ( value,
								              DEC_BASE );
								    __buf_putnumber
								            ( value,
								              DEC_BASE,
								              &buf,
								              nlen );

								    fmt++;
								    break;
							    }

							    /* Hexdecimal unsigned long long */
						    case 'x':
							    {
								    ul64 value =
								            ( ul64 )
								            va_arg
								            ( args,
								              ul64 );
								    nlength nlen
								            =
								            numlen
								            ( value,
								              HEX_BASE );
								    __buf_putnumber
								            ( value,
								              HEX_BASE,
								              &buf,
								              nlen );

								    fmt++;
								    break;
							    }

						    default:
							    {
								    fmt--;
								    break;
							    }
						  }
					  }

					  /* Decimal unsigned long */
					  else if ( *( fmt + 1 ) == 'u' )
					  {
						  u64 value =
						          ( u64 ) va_arg ( args,
						                           u64 );
						  nlength nlen = numlen ( value,
						                          DEC_BASE );

						  __buf_putnumber
						          ( value,
						            DEC_BASE,
						            &buf, nlen );
						  fmt++;
					  }

					  /* Hex unsigned long */
					  else if ( *( fmt + 1 ) == 'x' )
					  {

						  u64 value =
						          ( u64 ) va_arg ( args,
						                           u64 );
						  nlength nlen = numlen ( value,
						                          HEX_BASE );

						  __buf_putnumber
						          ( value,
						            HEX_BASE,
						            &buf, nlen );
						  fmt++;
					  }
					  break;
				  }

			  case 'p':    /* Pointer */
				  {
					  ul64 value = ( ul64 ) va_arg ( args,
					                                 ul64 );
					  nlength nlen = numlen ( value,
					                          HEX_BASE );

					  __buf_putnumber ( value,
					                    HEX_BASE,
					                    &buf, nlen );
					  break;

				  }

			  case 'x':    /* Hexadecimal */
				  {
					  u32 value = ( u32 ) va_arg ( args,
					                               u32 );
					  nlength nlen = numlen ( value,
					                          HEX_BASE );

					  __buf_putnumber ( value,
					                    HEX_BASE,
					                    &buf, nlen );
					  break;
				  }

			  case 'c':    /* Character */
				  {
					  char c = ( char ) va_arg ( args,
					                             int );

					  *buf++ = c;
					  break;
				  }

			  case 's':    /* String */
				  {
					  const char* str =
					          ( const char * )
					          va_arg ( args, long );

					  /* While it's not the final of string, copy its characters to the buffer */
					  for ( ; *str != '\0'; str++, buf++ )
						  *buf = *str;
					  break;
				  }

			  case '%':    /* Literal % */
				  {
					  *buf++ = *fmt;
					  break;
				  }

			  default:
				  {
					  *buf++ = ( const char ) '%';
					  *buf++ = *fmt;
					  break;
				  }
			}

			fmt++;
			continue;
		}

		/* Not a % */
		*buf++ = *fmt++;
	} while ( *fmt );
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
void
__buf_putnumber ( ul64 value, size_t base, buf_t** buf, nlength nlen )
{
	/* For decimal values, do not put the base indicator (0d) */
	nlength final_nlen = base == DEC_BASE ? nlen + 1 : nlen + 3;

	/*
	 * The following "+3" referes to:
	 * 1. 0 at start
	 * 2. Base indicator
	 * 3. \0 at the final
	 */
	buf_t buf_n[final_nlen];

	/* Format number adding '0_' at start */
	buf_n[0] = '0';
	buf_n[1] = base == BIN_BASE ? 'b'
	        : base == OCT_BASE ? 'o' : base == HEX_BASE ? 'x' : '?';

	/* Convert the value to string and copy it to given buffer */
	itoa ( value, base, buf_n, final_nlen );
	for ( u32 i = 0; buf_n[i] != '\0'; i++, ( *buf )++ )
		**buf = buf_n[i];
}
