#ifndef LOG_H
#define LOG_H

#include <rks/types.h>

#define DEFAULT_LOG_BUF_SZ (4096 * 5)

enum log_levels
{
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERR,
};

enum log_flags
{
	LOG_VGA_PRT = 1 << 0,
	LOG_APPEND  = 1 << 1,
};

void _log_init(void);
void _log(enum log_levels level, enum log_flags flags, const char *fmt, ...);

#define pr_log(level, fmt, ...) _log(level, LOG_VGA_PRT | LOG_APPEND, fmt, ##__VA_ARGS__)
#define INF(fmt, ...)           pr_log(LOG_INFO, fmt, ##__VA_ARGS__)
#define DBG(fmt, ...)           pr_log(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define WRN(fmt, ...)           pr_log(LOG_WARN, fmt, ##__VA_ARGS__)
#define ERR(fmt, ...)           pr_log(LOG_ERR, fmt, ##__VA_ARGS__)

#endif /* LOG_H */
