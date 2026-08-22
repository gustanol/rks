#ifndef _KERNEL_SUBSYS_PMM_BITMAP_BACKEND_H
#define _KERNEL_SUBSYS_PMM_BITMAP_BACKEND_H

#include <stddef.h>
#include <stdint.h>

/* Bitmap useful content data */
struct bitmap_ctx
{
	char* ptr;
	size_t size;
};

/* Global context struct */
static struct bitmap_ctx g_bitmap;

#endif /* _KERNEL_SUBSYS_PMM_BITMAP_BACKEND_H */
