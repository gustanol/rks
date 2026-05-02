#ifndef ARCH_CONST
#define ARCH_CONST

#define PAGE_LSHIFT 12
#define PAGE_SIZE ( 1ULL << ( PAGE_LSHIFT ) )

#ifndef __ASSEMBLY__
extern void kernel_start ( void );
extern void kernel_end ( void );
extern void KERNEL_VIRT_BASE ( void );
extern void KERNEL_PHYS_BASE ( void );
extern void KERNEL_PHYSMAP_BASE ( void );
#endif /* __ASSEMBLY__ */

#endif /* ARCH_CONST */
