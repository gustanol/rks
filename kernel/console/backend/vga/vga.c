#include <console/backend.h>
#include <kernel/mem/alloc.h>

static CONSOLE_BACKEND_INIT ( __vga_init );
static CONSOLE_BACKEND_PUTCHAR ( _vga_putchar );
static CONSOLE_BACKEND_WRITE ( _vga_write );
static CONSOLE_BACKEND_FLUSH ( _vga_flush );
static CONSOLE_BACKEND_CLEAR ( _vga_clear );

/* VGA operations */
static const struct console_ops vga_ops = {
	.putchar = _vga_putchar,
	.write = _vga_write,
	.flush = _vga_flush,
	.clear = _vga_clear,
};

/* VGA backend descriptor */
struct console_backend vga_backend = {

	/* *INDENT-OFF* */

	.base = {
		 .name = "vga_console",
		 .tag = SUBSYS_TAG_CONSOLE,
	},

	/* *INDENT-ON* */

	.init = __vga_init,
	.ops = &vga_ops,
};

/* Register this backend */
REGISTER_CONSOLE_BACKEND ( vga_backend );

/* Static VGA context */
static struct vga_ctx g_vga;

/**
 * VGA backend initialization
 *
 * Implements the console backend initialization interface
 *
 * Interface:
 *     `kernel/console/include/console/backend.h`
 **/
CONSOLE_BACKEND_INIT ( __vga_init )
{

	/* Defines the screen size */
	g_vga.screen.cell_count = VGA_HEIGHT * VGA_WIDTH;

	/**
         * Allocates a modifiable screen
         *
         * NOTE: `g_vga.screen.ptr` -> Pointer to a pointer of `struct vga_cell`
         * It's gonna be used as an array of cells
         **/
	g_vga.screen.ptr =
	        k_malloc ( sizeof ( struct vga_cell * ) *
	                   g_vga.screen.cell_count );

	KERR_RET_IF ( IS_KERR ( g_vga.screen.ptr ),
	              GET_KERR ( g_vga.screen.ptr ),
	              "Error allocating VGA screen" );

	/* Memory address to flush screen content */
	g_vga.maddr = VGA_MADDR;

	/* Ready to be used */
	g_vga.ready = true;

	/* Correctly initialized */
	return KERR_OK;
}
