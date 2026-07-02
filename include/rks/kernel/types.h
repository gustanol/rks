#ifndef _KERNEL_TYPES_H
#define _KERNEL_TYPES_H

#define NULL ( ( void * ) 0 )

/* Booleans */
typedef _Bool bool;

#define true  1
#define false 0

/* Unsigned types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/* Signed types */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

/* Explicit byte types */
typedef u8 byte_t;

/* Addresses */
typedef u32 addr32_t;

/* Type for registers */
typedef u64 reg_t;

/* System Types */
typedef u64 size_t;
typedef s64 ssize_t;
typedef u64 reg_t;
typedef u16 pad_t;

/* Pointer to a generic function */
typedef void ( *init_fn_t ) ( void*  );

#endif /* _KERNEL_TYPES_H */
