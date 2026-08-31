const std = @import("std");
const builtin = @import("builtin");
const Flag = @import("shared.zig").Flag;

const kernel_build = @import("kernel/kernel.build.zig");
const libc_build = @import("libc/libc.build.zig");

// Global CFLAGS that will be shared between all C/Zig files
const global_cflags = &[_][]const u8{
    "-ffreestanding",
    "-fno-stack-protector",
    "-fno-pic",
    "-fno-pie",
    "-mno-red-zone",
    "-Wall",
    "-Wextra",
    "-std=gnu11", // GNU extensions
    "-D__KERNEL__",
};

// Global assembly flags
const global_asflags = &[_][]const u8{
    "-D__ASSEMBLY__",
};

pub fn build(b: *std.Build) void {

    // Architecture selection
    const arch = b.option(std.Target.Cpu.Arch, "arch", "Override target CPU architecture")
    orelse builtin.cpu.arch;

    // This target options
    const target = b.resolveTargetQuery(.{
        .cpu_arch = arch, // Chosen architecture
        .os_tag = .freestanding,
        .abi = .none,
    });

    // Optimize options
    const optimize = b.standardOptimizeOption(.{});

    // Executable options
    const kernel = b.addExecutable(.{
        .name = "rks",
        .target = target,
        .optimize = optimize,
    });

    // Group the flags
    const flags: Flag = .{
        .asflags = global_asflags,
        .cflags = global_cflags,
    };

    // Top-level include folder
    kernel.root_module.addIncludePath(b.path("include"));

    kernel_build.addKernel(b, kernel, flags);
    libc_build.addLibrary(b, kernel, target, optimize);
    b.installArtifact(kernel);
}
