#include <terminal/backend.h>
#include <kernel/mem/alloc.h>

static TERMINAL_BACKEND_INIT ( __vga_init );
static TERMINAL_BACKEND_PUTCHAR ( _vga_putchar );
static TERMINAL_BACKEND_WRITE ( _vga_write );
static TERMINAL_BACKEND_FLUSH ( _vga_flush );
static TERMINAL_BACKEND_CLEAR ( _vga_clear );

/* VGA operations */
static const struct terminal_ops vga_ops = {
	.putchar = _vga_putchar,
	.write = _vga_write,
	.flush = _vga_flush,
	.clear = _vga_clear,
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

	/* Set VGA screen dimensions */
	g_vga.screen.attr.width = VGA_WIDTH;
	g_vga.screen.attr.height = VGA_HEIGHT;

	/* Defines the screen size */
	g_vga.screen.attr.cell_count =
	        g_vga.screen.attr.width * g_vga.screen.attr.height;

	/**
         * Allocates a modifiable screen
         *
         * NOTE: `g_vga.screen.ptr` -> Pointer to a pointer of `struct vga_cell`
         * It's gonna be used as an array of cells
         **/
	g_vga.screen.ptr =
	        k_malloc ( sizeof ( struct vga_cell ) *
	                   g_vga.screen.attr.cell_count );

	KERR_RET_IF ( IS_KERR ( g_vga.screen.ptr ),
	              GET_KERR ( g_vga.screen.ptr ),
	              "Error allocating VGA screen" );

	/* Set the default color */
	g_vga.screen.attr.current_color = VGA_DEFAULT_COLOR;

	/* Set initial coordinates */
	g_vga.screen.coordinates.x = 0;
	g_vga.screen.coordinates.y = 0;

	/* Memory address to flush screen content */
	g_vga.maddr = VGA_MADDR;

	/* Ready to be used */
	g_vga.ready = true;

	/* Correctly initialized */
	return KERR_OK;
}

TERMINAL_BACKEND_PUTCHAR ( _vga_putchar )
{
}
