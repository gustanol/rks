#ifndef _X86_CPU_H
#define _X86_CPU_H

#include <stddef.h>
#include <stdint.h>

/* CPU hardware state */
struct cpu_hw
{

	/* LAPIC ID */
	uint32_t lapic_id;

	/* Logical index (0, 1, ...) */
	uint32_t idx;

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
	uint64_t tss_base;

	/* Stack pointers */
	uint64_t ist_stacks[7];

	/**
	 * Nesting depth of interrupt handlers
	 * How many IRQs can be pending at once
	 **/
	uint8_t irq_depth;
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

#endif /* _X86_CPU_H */
