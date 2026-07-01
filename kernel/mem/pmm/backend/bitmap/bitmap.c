#include "bitmap.h"
#include <arch/const.h>
#include <arch/mem/early_alloc.h>
#include <pmm/backend.h>
#include <rks/kernel/init/subsystem.h>
#include <rks/kernel/mem/vmm/types.h>
#include <rks/lib/bitwise.h>

/* Function prototypes
 * =================== */
static void __bitmap_init ( const struct pmm_init_backend_ctx* ctx );

/* Operations
 * ========== */
static paddr_t _bitmap_alloc_frame ( u32 total_frames );
static kerr_t _bitmap_free_frame ( paddr_t phys_addr, u32 total_frames );
static u32 _bitmap_free_region ( paddr_t phys_addr, size_t size,
                                 u32 total_frames );
static u32 _bitmap_fill_region ( paddr_t phys_addr, size_t size,
                                 u32 total_frames );

/* Global context struct */
static struct bitmap_ctx g_bitmap;

/* PMM operations */
static const struct pmm_ops bitmap_ops = {
	.alloc_frame = _bitmap_alloc_frame,
	.free_frame = _bitmap_free_frame,
	.free_region = _bitmap_region_free,
	.fill_region = _bitmap_region_used,
};

/* Register it as a PMM backend */
REGISTER_PMM_BACKEND ( "bitmap_pmm", __bitmap_init, &bitmap_ops );

/**
 * PMM bitmap backend initializer function
 * Subsystem backend init function
 *
 * Return -> `void`
 * Arguments:
 *   1. `const struct pmm_init_backend_ctx* ctx`: PMM non-specific backend data
 * Callers:
 *   - `pmm_init`
 **/
void
__bitmap_init ( const struct pmm_init_backend_ctx* ctx )
{
	/*
	 * Calculates the bitmap size in bytes
	 *
	 * NOTE: One block is represented by one bit
	 * We need to convert it to byte size
	 */
	g_bitmap.size = BIT_TO_BYTE_SIZE ( ctx->frames_total );

	/* Allocates the bitmap pointer */
	g_bitmap.ptr = ( char * ) early_alloc ( g_bitmap.size );

	/* Sets this region as used */
	_bitmap_fill_region ( ( paddr_t )
	                      ( ( vaddr_t ) g_bitmap.ptr -
	                        ( vaddr_t ) HHDM_BASE ), g_bitmap.size );
}

/**
 * Bitmap single frame allocator
 * Subsystem backend function
 *
 * Return -> `paddr_t`: Allocated page physical address
 * Arguments:
 *   1. `u32 total_frames`: Total amount of memory frames
 * Callers:
 *   - PMM implementations
 **/
paddr_t
_bitmap_alloc_frame ( u32 total_frames )
{
	/* Gets the index of the first available physical frame */
	u32 frame_idx = FIND_FIRST_ZERO_BIT ( g_bitmap.ptr, g_bitmap.size );

	/* No available memory */
	if ( frame_idx < 0 )
		return ENCODE_KERR ( KERR_NOMEM );

	/* Frame out of boundary */
	if ( frame_idx > total_frames )
		return ENCODE_KERR ( KERR_INTERNAL );

	/* Mark this frame as used */
	BITMAP_SET_BIT ( g_bitmap.ptr, frame_idx );

	/* Verify if the bit was set */
	if ( !BITMAP_TEST_BIT ( g_bitmap.ptr, frame_idx ) )
		return ENCODE_KERR ( KERR_INTERNAL );

	/* Page physical address */
	return ( frame_idx * FRAME_SIZE );
}

/**
 * Bitmap single frame freer
 * Subsystem backend function
 *
 * Return -> `kerr_t`: Global error type
 * Arguments:
 *   1. `paddr_t phys_addr`: Frame physical address
 *   2. `u32 total_frames`: Total amount of physical frames
 * Callers:
 *   - PMM implementations
 **/
kerr_t
_bitmap_free_frame ( paddr_t phys_addr, u32 total_frames )
{
	/* Physical frame index */
	u32 frame_idx = phys_addr / FRAME_SIZE;

	/* Invalid physical address */
	if ( frame_idx > total_frames )
		return KERR_INVAL;

	/* Mark this frame as used */
	BITMAP_CLEAR_BIT ( g_bitmap.ptr, frame_idx );

	/* Verify if the bit was cleared */
	if ( BITMAP_TEST_BIT ( g_bitmap.ptr, frame_idx ) )
		return KERR_INTERNAL;

	/* Otherwise, success */
	return KERR_OK;
}

/**
 * Bitmap region map freer
 * Subsystem backend function
 *
 * Return -> `u32`: Successfully freed frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 *   3. `u32 total_frames`: Total frames provided by dispatch layer
 * Callers:
 *   - PMM imprementations
 **/
u32
_bitmap_free_region ( paddr_t phys_addr, size_t size, u32 total_frames )
{
	/* Starting index of the physical frame */
	u32 frame_idx = phys_addr / FRAME_SIZE;

	/* Amount of frames this region has */
	u32 frame_count = size / FRAME_SIZE;

	/* Region out of boundary */
	if ( frame_idx + frame_count > total_frames )
		return ENCODE_KERR ( KERR_INVAL );

	/* Clear this region on the bitmap */
	u32 cleared_frames =
	        BITMAP_CLEAR_REGION ( g_bitmap.ptr, frame_idx, frame_count );

	/* The amount of frames that were correctly cleared */
	return cleared_frames;
}

/**
 * Bitmap region map setter
 * Subsystem backend function
 *
 * Return -> `u32`: Successfully set frames
 * Arguments:
 *   1. `paddr_t phys_addr`: Region base physical address
 *   2. `size_t size`: Region size
 *   3. `u32 total_frames`: Total frames provided by dispatch layer
 * Callers:
 *   - PMM imprementations
 **/
u32
_bitmap_fill_region ( paddr_t phys_addr, size_t size, u32 total_frames )
{
	/* Physical frame starting index */
	u32 frame_idx = phys_addr / FRAME_SIZE;

	/* Frames amount */
	u32 frame_count = size / FRAME_SIZE;

	/* Region out of boundary */
	if ( frame_idx + frame_count > total_frames )
		return ENCODE_KERR ( KERR_INVAL );

	/* Set all the bits of this region */
	u32 set_frames =
	        BITMAP_SET_REGION ( g_bitmap.ptr, frame_idx, frame_count );

	/* The amount of frames that were correctly set */
	return set_frames;
}
