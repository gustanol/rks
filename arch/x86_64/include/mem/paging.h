#ifndef PAGING_H
#define PAGING_H

#include <rks/kernel/mem/vmm.h>

/* Page Table Entry bits */
#define PTE_PRESENT ( 1ULL << 0 )       /* Is present */
#define PTE_RW ( 1ULL << 1 )    /* Read/Write */
#define PTE_USER ( 1ULL << 2 )
#define PTE_ACCESSED ( 1ULL << 5 )
#define PTE_DIRTY ( 1ULL << 6 )
#define PTE_HUGE ( 1ULL << 7 )  /* 2 MiB pages */
#define PTE_XD ( 1ULL << 63 )   /* Execution disabled */

#define PML4_INDEX(vaddr) \
        ( ( ( vaddr ) >> 39 ) & 0x1FF)

#define PDPT_INDEX(vaddr) \
        ( ( ( vaddr ) >> 30 ) & 0x1FF)

#define PD_INDEX(vaddr) \
        ( ( ( vaddr ) >> 21 ) & 0x1FF)

#define PT_INDEX(vaddr) \
        ( ( ( vaddr ) >> 12 ) & 0x1FF)

#define ADDR_OFFSET(vaddr) \
        ( ( vaddr ) & 0xFFF )

#define PT_ENTRIES 512

/* Page table entries */
typedef addr_t pte_t;

/* It must be aligned to unconsider the flags */
typedef struct
{
	pte_t entries[PT_ENTRIES];
} __attribute__ ( ( aligned ( 4096 ) ) ) page_table_t;

typedef struct
{
	page_table_t* pml4;
	addr_t phys_pml4;
} paging_t;

typedef enum
{
	PPRESENT = 1 << 0,
	PRW = 1 << 1,
	PUS = 1 << 2,
	PCD = 1 << 3,
} paging_flags_t;

bool map_page ( vaddr_t vaddr, paddr_t paddr, paging_t * paging_ctx,
                u64 flags );
bool unmap_page ( vaddr_t vaddr, paging_t * paging_ctx );
paddr_t get_phys ( vaddr_t vaddr, paging_t * paging_ctx );

/* Testings */
void dump_tables ( vaddr_t vaddr, paging_t * paging_ctx );

#endif /* PAGING_H */
