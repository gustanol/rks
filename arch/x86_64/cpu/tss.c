#include <cpu/tss.h>
#include <rks/kernel/mem/vmm.h>

#define LOAD_TSS(DESCRIPTOR) \
        __asm__ volatile ( \
                "ltr %0" \
                : \
                : "r"(DESCRIPTOR) \
                : "memory" \
        )

/* Task State Segment */
tss_t tss;
extern void kernel_stack_top ( void );

/*
 * Setup and load a basic Task State Segment
 *
 * -> void
 * Called by `arch_eary_init`
 */
void
_setup_tss ( void )
{
	/* *INDENT-OFF* */

        /* Populate the TSS before load it */
        tss = ( tss_t )
        {
                .reserved0 = { 0 },
                .rsp0 = ( vaddr_t ) kernel_stack_top,

                /*
                 * .rsp1 = stack1
                 * .rsp2 = stack2
                 */

                .reserved1 = { 0 },
                .reserved2 = { 0 },
                .reserved3 = { 0 },
        };

        /* *INDENT-ON* */

	/* Load the TSS by its GDT descriptor index */
	LOAD_TSS ( 0x38 );
}
