#include <rks/log.h>
#include <rks/print.h>
#include <rks/string.h>

/* Log subsystem global state struct */
struct log_t
{
	char           *buf;
	__u64           buf_s;
	__u32           pos;
	enum log_levels log_level;
};

static char         buf[DEFAULT_LOG_BUF_SZ];
static struct log_t log_p;

static void __k_log_append(const char *fmt);

/*
 * Initializes log global state system
 *
 * -> `void`
 */
void _log_init(void)
{
	log_p.buf       = (char *)buf;
	log_p.buf_s     = DEFAULT_LOG_BUF_SZ;
	log_p.pos       = 0;
	log_p.log_level = LOG_INFO;
}

/*
 * Basic function to emit a log
 *
 * -> `void`
 * @level: Log level
 * @fmt: Log message
 * @...: Extra arguments
 */
void _log(enum log_levels level, enum log_flags flags, const char *fmt, ...)
{
	buf_t buf[FMT_BUF_SZ];

	va_list args;
	va_start(args, fmt);

	/* Format message */
	_fmt(buf, FMT_BUF_SZ, fmt, args);

	va_end(args);
	if (flags & LOG_KEEP)
		__k_log_append(buf);

	if (flags & LOG_PRINT)
	{
		if (level == LOG_DEBUG)
			__k_print_write("[DBG] ");
		else if (level == LOG_INFO)
			__k_print_write("[INF] ");
		else if (level == LOG_WARN)
			__k_print_write("[WRN] ");
		else if (level == LOG_ERR)
			__k_print_write("[ERR] ");

		__k_print_write(buf);
	}

	if (level)
		return;
}

static void __k_log_append(const char *fmt)
{
	/*
	 * Don't write if the string doesn't fit in remain offset
	 *
	 * Go back
	 */
	if (sizeof(fmt) > log_p.buf_s - log_p.pos)
		log_p.pos = 0;

	for (; *fmt != '\0'; log_p.pos++, fmt++)
		log_p.buf[log_p.pos] = *fmt;
}
