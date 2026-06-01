#ifndef ARCH_CONST
#define ARCH_CONST

#define PAGE_LSHIFT 12
#define PAGE_SIZE ( 1ULL << ( PAGE_LSHIFT ) )

#ifndef __ASSEMBLY__

extern void KERNEL_START ( void );
extern void KERNEL_END ( void );
extern void KERNEL_VIRT_BASE ( void );
extern void KERNEL_VIRT_OFF ( void );
extern void KERNEL_PHYS_BASE ( void );
extern void HHDM_BASE ( void );

#endif /* __ASSEMBLY__ */

#endif /* ARCH_CONST */
