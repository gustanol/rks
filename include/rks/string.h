#ifndef STRING_H
#define STRING_H

#include <rks/args.h>
#include <rks/types.h>

#define FMT_BUF_SZ 512

typedef char buf_t;

size_t itoa (__u64 num, __u32 base, buf_t * buf, size_t buf_s);
__u64 atoi (buf_t * buf, size_t buf_s);
void _fmt (buf_t * buf, size_t size, const char *fmt, va_list args);

#endif /* STRING_H */
