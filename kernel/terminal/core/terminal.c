#include <terminal/internal.h>
#include <kernel/terminal/init.h>
#include <kernel/mem/alloc.h>

struct terminal_ctx i_terminal;

kerr_t
terminal_init ( struct framebuffer_info* fb )
{

	g_terminal = &i_terminal;
	kerr err;

	/* Discover all registered backends */
	struct backends_list* backends =
	        subsys_find_multiple_backends ( SUBSYS_TAG_TERMINAL );

	KERR_RET_IF ( backends == NULL, KERR_INTERNAL,
	              "Error getting terminal backends" );

	KERR_RET_IF ( backends->count == 0, KERR_INTERNAL,
	              "No registered terminal backend" );

	g_terminal->backends.count = backends->count;

	g_terminal->backends.ops_list =
	        k_malloc ( sizeof ( struct terminal_ops * ) * backends->count );

	KERR_PROPAGATE ( g_terminal->ops_list );

	for ( uint32_t i = 0; i < backends->count; i++ )
	{
		struct terminal_backend* current_terminal_backend =
		        console_backend_from_base ( backends->arr[i] );

		err = current_terminal_backend->init ( fb );

		/*
		   if (!IS_RET_OK(err))
		   k_log(...);
		 */

		g_terminal->backends.list[i] = current_terminal_backend->ops;
	}

	err = k_mfree ( backends->arr );
	/*
	   if (!IS_RET_OK(err))
	   k_log(...);
	 */

	err = k_mfree ( backends );
	/*
	   if (!IS_RET_OK(err))
	   k_log(...);
	 */

	g_terminal->ready = true;
}
