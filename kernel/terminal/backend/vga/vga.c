#include <terminal/backend.h>
#include <kernel/mem/alloc.h>

static TERMINAL_BACKEND_INIT ( __vga_init );
static TERMINAL_BACKEND_WRITE_AT ( _vga_write_at );
static TERMINAL_BACKEND_PUTC_AT ( _vga_putc_at );
static TERMINAL_BACKEND_SET_STYLE ( _vga_set_style );
static TERMINAL_BACKEND_FLUSH ( _vga_flush );

/* VGA operations */
static const struct terminal_ops vga_ops = {
	.write_at = _vga_write_at,
	.putc_at = _vga_putc_at,
	.set_style = _vga_set_style,
	.flush = _vga_flush,
};

/* VGA backend descriptor */
struct terminal_backend vga_backend = {

	/* *INDENT-OFF* */

	.base = {
		 .name = "vga_terminal",
		 .tag = SUBSYS_TAG_TERMINAL,
	},

	/* *INDENT-ON* */

	.init = __vga_init,
	.ops = &vga_ops,
};

/* Register this backend */
REGISTER_TERMINAL_BACKEND ( vga_backend );

/* Static VGA context */
static struct vga_ctx g_vga;

/**
 * VGA backend initialization
 *
 * Implements the terminal backend initialization interface
 *
 * Interface:
 *   - `kernel/terminal/include/terminal/backend.h`
 **/
TERMINAL_BACKEND_INIT ( __vga_init )
{

	/**
	 * Allocates a modifiable screen
	 *
	 * NOTE: `g_vga.ptr` -> Pointer to a pointer of `struct vga_cell`
	 * It's gonna be used as an array of cells
	 **/
	struct vga_cell* ptr = k_malloc ( sizeof ( struct vga_cell ) * vga_cell_count );

	/* Treat error returnings */
	KERR_RET_IF ( IS_KERR ( ptr ), GET_KERR ( ptr ), "Error allocating VGA screen" );

	/* Set the allocated pointer */
	g_vga.ptr = ptr;

	/* Correctly initialized */
	return KERR_OK;
}

/**
 * VGA backend string buffer writer 
 *
 * Implements the terminal backend specific buffer writer interface
 *
 * Interface:
 *   - `kernel/terminal/include/terminal/backend.h`
 **/
TERMINAL_BACKEND_WRITE_AT ( _vga_write_at )
{
	if ( row >= vga_height )
		return;

	if ( col >= vga_width )
		return;

	/* Iterate up to `len` and call the internal `putc_at` */
	for ( uint32_t i = 0; i < len; i++ )
		_vga_putc_at ( row, col + i, s[i] );
}

/**
 * VGA backend character writer
 *
 * Implements the terminal backend specific character writer interface
 *
 * Interface:
 *   - `kernel/terminal/include/terminal/backend.h`
 **/
TERMINAL_BACKEND_PUTC_AT ( _vga_putc_at )
{

	/* Do not write over the last row */
	if ( row >= vga_height )
		return;

	/* Do not write over the last col */
	if ( col >= vga_width )
		return;

	/* Set the character and its attributes */
	g_vga.ptr[row * col].character = c;
	g_vga.ptr[row * col].color = g_vga.current_color;
}
