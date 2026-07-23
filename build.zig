const std = @import("std");

/// Child directories
const kernel_build = @import("kernel/kernel.build.zig");

pub fn build(b: *std.Build) void {

    // Architecture selection
    const arch_opt = b.option
        ([]const u8,
            "arch",
            "Target architecture (x86, aarch64)"
        ) orelse "x86";

    // Select chosen architecture from the enum `std.Target.Cpu.Arch
    const cpu_arch =
        std.meta.stringToEnum(std.Target.Cpu.Arch, arch_opt)
        orelse .x86;

    // This target options
    const target = b.resolveTargetQuery(.{
        .cpu_arch = cpu_arch, // Chosen architecture
        .os_tag = .freestanding,
        .abi = .none,
    });

    // Executable options
    const kernel_exec = b.addExecutable(.{
        .name = "rks",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = b.standardOptimizeOption(.{}),
        }),
    });

    // Global CFLAGS that will be shared between all C/Zig files
    const global_cflags = &[_][]const u8{
        "-ffreestanding",
        "-fno-stack-protector",
        "-fno-pic",
        "-fno-pie",
        "-mno-red-zone",
        "-mcmodel=kernel",
        "-Wall",
        "-Wextra",
        "-std=gnu11", // GNU extensions
    };

    kernel_build.addKernel(b, kernel_exec, global_cflags);
    b.installArtifact(kernel_exec);
}
