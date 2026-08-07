#include <terminal/internal.h>
#include <kernel/terminal/init.h>
#include <kernel/mem/alloc.h>

/* Internal static terminal implementation */
struct terminal i_terminal;

/**
 * Terminal subsystem initializer function
 * Subsystem init function
 *
 * Return -> `kerr_t`: Kernel global error type
 * Arguments:
 *   1. `struct framebuffer_info* fb`
 **/
kerr_t
terminal_init ( struct framebuffer_info* fb )
{

	g_terminal = &i_terminal;

	/* Discover all registered backends */
	struct backends_list* backends = subsys_find_multiple_backends ( SUBSYS_TAG_TERMINAL );

	/* Exceptions treatments */
	KERR_RET_IF ( backends == NULL, KERR_INTERNAL, "Error getting terminal backends" );
	KERR_RET_IF ( backends->count == 0, KERR_INTERNAL, "No registered terminal backend" );

	/* Set extracted backend-specific data */
	g_terminal->backends.count = backends->count;
	struct terminal_ops* ops_list = k_malloc ( sizeof ( struct terminal_ops * ) * backends->count );

	/* Set the operations list after verificaton */
	KERR_PROPAGATE ( ops_list );
	g_terminal->backends.ops_list = ops_list;

	/* Initialize each found backend */
	for ( uint32_t i = 0; i < backends->count; i++ )
	{

		/* Current Terminal-specific struct from the current backend */
		struct terminal_backend* current_terminal_backend =
		    console_backend_from_base ( backends->arr[i] );

		/* Initializes this backend */
		kerr_t err = current_terminal_backend->init ( fb );

		/**
         * Set operations for correctly initialized backends
         * Note it's not needed every of them to be initialized
         **/
		if ( IS_RET_OK ( err ) )
			g_terminal->backends.list[i] = current_terminal_backend->ops;
	}

	k_mfree ( backends->arr );  /* Array of backends' struct context */
	k_mfree ( backends );       /* Higher-layer struct pointer */

	/* Initializes manager fucntions */
	kerr_t ret = terminal_manager_init (  );
	KERR_PROPAGATE ( ret );

	/* Subsystem is ready */
	g_terminal->ready = true;
}
