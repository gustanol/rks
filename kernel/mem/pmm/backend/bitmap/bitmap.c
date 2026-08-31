#include "bitmap.h"
#include <arch/const.h>
#include <arch/mem/early_alloc.h>
#include <lib/bitwise.h>
#include <kernel/mem/pmm/frames.h>
#include <kernel/mem/vmm/types.h>
#include <pmm/backend.h>

/* Prototypes */
static PMM_BACKEND_INIT ( __bitmap_init );
static PMM_BACKEND_ALLOC_FRAME ( _bitmap_alloc_frame );
static PMM_BACKEND_FREE_FRAME ( _bitmap_free_frame );
static PMM_BACKEND_ALLOC_REGION ( _bitmap_alloc_region );
static PMM_BACKEND_FILL_REGION ( _bitmap_fill_region );
static PMM_BACKEND_FREE_REGION ( _bitmap_free_region );

/* PMM operations */
static const struct pmm_ops bitmap_ops = {
	.alloc_frame = _bitmap_alloc_frame,
	.free_frame = _bitmap_free_frame,
	.alloc_region = _bitmap_alloc_region,
	.fill_region = _bitmap_fill_region,
	.free_region = _bitmap_free_region,
};

/* PMM backend context structure */
static const struct pmm_backend bitmap_backend = {

	/* *INDENT-OFF* */

	.base = {
		 .name = "pmm_bitmap",
		 .tag = SUBSYS_TAG_PMM,
	},

	/* *INDENT-ON* */

	.init = __bitmap_init,
	.ops = &bitmap_ops,
};

/* Register it as a PMM backend */
REGISTER_PMM_BACKEND ( bitmap_backend );

/**
 * Bitmap backend initialization
 *
 * Implements the PMM backend initialization interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_BACKEND_INIT ( __bitmap_init )
{

	/**
	 * Bitmap size in bytes
	 *
	 * NOTE: One block is represented by one bit
	 * We need to convert it to byte size
	 **/
	g_bitmap.size = BIT_TO_BYTE_SIZE ( ctx->total_frames );

	/* Allocate the bitmap */
	g_bitmap.ptr = ( char * ) early_alloc ( g_bitmap.size );

	/* Sets this region as used */
	_bitmap_fill_region ( g_bitmap.ptr, g_bitmap.size, ctx->total_frames );

	/* Everything's okay */
	return KERR_OK;
}

/**
 * Bitmap backend frame allocator
 *
 * Implements the PMM backend frame allocator interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_ALLOC_FRAME ( _bitmap_alloc_frame )
{

	/* Index of the first available physical frame */
	long frame_idx = FIND_FIRST_ZERO_BIT ( g_bitmap.ptr, g_bitmap.size );

	/* No available memory */
	KERR_RET_ENCODE_IF ( frame_idx < 0, KERR_NOMEM, "No free physical frame\n" );

	/* Frame out of boundary */
	KERR_RET_ENCODE_IF ( frame_idx > total_frames, KERR_INTERNAL,
	    "Frame '%l' out of bounds\n", frame_idx );

	/* Mark this frame as used */
	BITMAP_SET_BIT ( g_bitmap.ptr, frame_idx );

	/* Page physical address */
	return ( frame_idx * FRAME_SIZE );
}

/**
 * Bitmap backend frame liberator
 *
 * Implements the PMM backend frame liberator interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_BACKEND_FREE_FRAME ( _bitmap_free_frame )
{

	/* Physical frame index */
	uint32_t frame_idx = phys_addr / FRAME_SIZE;

	/* Frame out of boundary */
	KERR_RET_ENCODE_IF ( frame_idx > total_frames, KERR_INVAL,
	    "Frame '%l' out of bounds\n", frame_idx );

	/* Mark this frame as used */
	BITMAP_CLEAR_BIT ( g_bitmap.ptr, frame_idx );

	/* Success */
	return KERR_OK;
}

/**
 * Bitmap backend region allocator 
 *
 * Implements the PMM backend region allocator interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_BACKEND_ALLOC_REGION ( _bitmap_alloc_region )
{

	/* First region frame index */
	long region_start = FIND_CONTIGUOUS_ZERO_REGION ( g_bitmap.ptr, g_bitmap.size,
	    frames_needed );

	/* No available memory */
	KERR_RET_ENCODE_IF ( region_start < 0, KERR_NOMEM, "No free physical frame\n" );

	/* Mark the found region as used */
	BITMAP_SET_REGION ( g_bitmap.ptr, region_start, frames_needed );

	/* Return the starter physical address */
	return ( region_start * FRAME_SIZE );
}

/**
 * Bitmap backend region filler
 *
 * Implements the PMM backend region filler interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_BACKEND_FILL_REGION ( _bitmap_fill_region )
{

	/* Physical frame starting index */
	uint32_t frame_idx = phys_addr / FRAME_SIZE;

	/* Frames amount */
	uint32_t frame_count = FRAMES_BY_SIZE ( size );

	/* Region out of boundary */
	KERR_RET_IF ( frame_idx + frame_count > total_frames, KERR_INVAL,
	    "Limit '%l' out of bounds\n", frame_idx + frame_count );

	/* Set all the bits of this region */
	BITMAP_SET_REGION ( g_bitmap.ptr, frame_idx, frame_count );

	/* The amount of frames that were correctly set */
	return KERR_OK;
}

/**
 * Bitmap backend region freer
 *
 * Implements the PMM backend region freer interface
 *
 * Interface:
 *   - `kernel/mem/pmm/include/pmm/backend.h`
 **/
PMM_BACKEND_FREE_REGION ( _bitmap_free_region )
{

	/* Starting index of the physical frame */
	uint32_t frame_idx = phys_addr / FRAME_SIZE;

	/* Amount of frames this region has */
	uint32_t frame_count = size / FRAME_SIZE;

	/* Region out of boundary */
	KERR_RET_ENCODE_IF ( frame_idx + frame_count > total_frames, KERR_INVAL,
	    "Limit '%l' out of bounds", frame_idx + frame_count );

	/* Clear this region on the bitmap */
	BITMAP_CLEAR_REGION ( g_bitmap.ptr, frame_idx, frame_count );

	/* The amount of frames that were correctly cleared */
	return KERR_OK;
}
