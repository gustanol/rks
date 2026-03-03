# x64 entry

The initial execution mode, usually right after the bootloader jump, is restricted and does not provide full 64-bit capabilities. The most of modern bootloaders load kernels in 32-bit **protected mode**, what means, for a full 64-bit kernel, it must set by itself.

> [!WARNING]
> Be aware that, for any reason, you try to load this kernel (at least, for now) using a bootloader that does not provide a 32-bit x86 environment, this kernel simply won't load.
> Although 16-bit bootloaders are not that common nowadays (except for initial code), it's strongly recommend you to use **GRUB** or another one that supports Multiboot2 loads.

---

The kernel linker script defines the `multiboot` section as the first one in the binary. It's not a required practice, but as you probably have seen at [boot-sequence](../../boot-sequence.md), the Multiboot2 header must be placed at the first 32 KiB of kernel binary. So, putting it at a dedicated section is a way of keeping things clearer.

The header has the following C declaration:
```c
struct multiboot_header {
    int magic;
    int arch;
    int header_length;
    int checksum;

    /* Several fields depending set flags */
    short flags;
};
```

And the following definition in `GAS` (GNU Assembler):
```asm
multiboot_header_start:
	.long 0xE85250D6
	.long 0x00000000 # i386
	.long multiboot_header_end - multiboot_header_start
	.long -(0xE85250D6 + 0x00 + (multiboot_header_end - multiboot_header_start))
	.short 0 # Tag type = 0 (terminator)
	.short 0 # Flags = 0
	.long 8 # Size = 8 (terminator size)
multiboot_header_end:
```

---

Finally, the initial kernel point, defined as `__rks_start` is reached.

Some important aspects from now:
- The 32-bit register `eax` contains the Multiboot2 bootloader magic, put by the bootloader as a verification method;
- `ebx` contains the base address of the Multiboot header structure.

Now, notice that a basic initial 16-MiB stack memory is defined and loaded. It'll be properly used only on functions calls.

After this setup, the next stage (32-bit GDT setup) is jumped to.
