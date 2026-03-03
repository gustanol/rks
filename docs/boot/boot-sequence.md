# Boot sequence

## Phase 0 - Initialization

The `Application Binary Interface (ABI)` object file format used, by default, to build this kernel is the `Executable and Linkable Format (ELF)`. It organizes structurally how things should be seen by the loader before control passing.

What matters here is where the entry point and the `Multiboot2` header are placed.

The kernel entry point will depend the architecture (in build time), but the Multiboot2 header must be placed within 32 KiB of the binary, as specified in [Multiboot2 specification](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html).

## Phase 1 - Transfer of control

From here, after the loader's jump, this kernel can assume the control. This phase, before independent-architectural code, is usually intentionally kept small, with a minimal execution environment.

It usually does not guarantee a safe execution under any unexpected/unsupported result.

## Phase 2 - Establish execution invariants

This phase exists at all to set, usually properties, that are going to remain unchanged through kernel operation.

It includes architecture-specific boot code to get things work properly before generic kernel code.

## Phase 3 - Transition to independent-architecture code

Finally, after those phases, the transition can happen. It's a phase to initialize subsystems, introduce new features and all the kernel must have to work.
