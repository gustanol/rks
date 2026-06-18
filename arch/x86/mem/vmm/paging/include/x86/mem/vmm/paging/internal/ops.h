#ifndef X86_PAGING_OPS
#define X86_PAGING_OPS

#include <rks/kernel/mem/pmm.h>

/* Address space struct */
struct addr_space
{
	paddr_t pml4_phys;
};

/* VMM backend functions */
ADDR_SPACE* _paging_create_space ( void );
void _paging_destroy_space ( ADDR_SPACE * space );
void _paging_switch_to_space ( ADDR_SPACE * space );

int _paging_map ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
                  vaddr_t hhdm_offset, u32 flags );
int _paging_unmap ( ADDR_SPACE * space, vaddr_t virt_addr,
                    vaddr_t hhdm_offset );
int _paging_remap ( ADDR_SPACE * space, vaddr_t virt_addr, paddr_t phys_addr,
                    vaddr_t hhdm_offset, u32 flags );

paddr_t _paging_virt_to_phys ( ADDR_SPACE * space, vaddr_t virt_addr,
                               vaddr_t hhdm_offset );
vaddr_t _paging_phys_to_virt ( paddr_t phys_addr, vaddr_t hhdm_offset );

void _paging_flush_tlb ( ADDR_SPACE * space, vaddr_t virt_addr, size_t size );

#endif /* X86_PAGING_OPS */
