#include <cpu/descriptors/tss.h>
#include <mem/early_alloc.h>

/* Helper macro used to load a new TSS
 * =================================== */
#define LOAD_TSS(DESCRIPTOR) \
        __asm__ volatile ( \
                "ltr %0" \
                : \
                : "r"(DESCRIPTOR) \
                : "memory" \
        )

/*
 * Setups and loads a 64-bit Task State Segment
 * Public helper
 *
 * Return -> `void`
 * Arguments:
 *   1. `void`
 * Callers:
 *   - `set_64_descriptors`
 */
void
_setup_tss ( void )
{
	/* TSS allocation */
	struct tss* tss =
	        ( struct tss * ) early_alloc ( sizeof ( struct tss ) );

	/* Top of the current kernel stack memory */
	extern void kernel_stack_top ( void );

	/* *INDENT-OFF* */

        /* Populates the TSS before loading it */
        tss = ( struct tss )
        {
                .reserved0 = { 0 },
                .rsp0 = ( vaddr_t ) kernel_stack_top,

                /*
                 * .rsp1 = stack1
                 * .rsp2 = stack2
                 *
                 * In future, you can allocate other stacks right here
                 * Commonly, using `early_alloc` or any other method
                 */

                .reserved1 = { 0 },
                .reserved2 = { 0 },
                .reserved3 = { 0 },
        };

        /* *INDENT-ON* */

	/* Loads the TSS by its GDT descriptor index */
	LOAD_TSS ( 0x38 );
}
