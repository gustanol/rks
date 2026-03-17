#ifndef TYPES_H
#define TYPES_H

#define NULL (void*)0
#define true 1;
#define false 0;

typedef _Bool bool;

/* Unsigned common types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned long long ul64;

/* If needed, explicit signed types */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;
typedef signed long long sl64;

typedef s8 byte_t;
typedef u8 ubyte_t;

/* Type used for addresses */
typedef u64 addr_t;

/*
 * 16-bit padding type
 *
 * Can be used as an array
 */
typedef u16 pad_t;

typedef u64 size_t;
typedef s64 ssize_t;

#endif /* TYPES_H */
