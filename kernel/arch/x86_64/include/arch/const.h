#ifndef _X86_64_CONST_H
#define _X86_64_CONST_H

#ifndef __ASSEMBLY__

extern void KERNEL_START ( void );
extern void KERNEL_END ( void );
extern void KERNEL_VIRT_BASE ( void );
extern void KERNEL_VIRT_OFF ( void );
extern void KERNEL_PHYS_BASE ( void );
extern void HHDM_BASE ( void );

#endif /* __ASSEMBLY__ */

#endif /* _X86_64_CONST_H */
