#ifndef STRING_H
#define STRING_H

#include <rks/types.h>

typedef __u8 buf_t;

size_t itoa(__u64 num, __u32 base, buf_t *buf, size_t buf_s);

#endif /* STRING_H */
