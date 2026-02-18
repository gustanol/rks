#include <rks/args.h>
#include <rks/console/console.h>
#include <rks/print.h>
#include <rks/string.h>

/*
 * Prints a formatted text to screen
 *
 * -> `void`
 * @fmt: The string
 * @...: Extra arguments
 */
void
kprintf (const char* fmt, ...)
{
	buf_t buf[FMT_BUF_SZ];

	/* Extra arguments */
	va_list args;

	va_start (args, fmt);

	/* Format the string */
	_fmt (buf, FMT_BUF_SZ, fmt, args);
	va_end (args);

	__k_print_write (buf);
}

void
__k_print_write (char* fmt)
{
	/* Write to screen */
	console_write (fmt);
	console_flush ();
}
