#ifndef CPU_H
#define CPU_H

#include <rks/kernel/types.h>
#include <x86/mem/vmm/paging.h>

/* CPU hardware state */
struct cpu_hw
{
	/* LAPIC ID */
	u32 lapic_id;

	/* Logical index (0, 1, ...) */
	u32 idx;

	/* Has this CPU core been brought up? */
	bool online;
};

/* CPU execution state */
struct cpu_exec
{
	/* struct process* process; */
	/* struct thread* thread; */

	/* Current address space for this core */
	struct addr_space* active_space;
};

/*
 struct cpu_sched {};
 */

/* CPU state for Interrupt Requests */
struct cpu_irq
{
	/* Probably, this section can be replaced by a per-CPU TSS state */

	/* Task State Segment base */
	vaddr_t tss_base;

	/* Stack pointers */
	vaddr_t ist_stacks[7];

	/*
	 * Nesting depth of interrupt handlers
	 *
	 * How many IRQs can be pending at once
	 */
	u8 irq_depth;
};

/* CPU core state */
struct cpu
{
	struct cpu_hw hardware;
	struct cpu_exec execution;

	/* struct cpu_sched sched; */

	struct cpu_irq irq;

	/* Self-reference pointer */
	struct cpu* self;
};

#endif /* CPU_H */
