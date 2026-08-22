const std = @import("std");

const boot = @import("boot/boot.build.zig");
const cpu = @import("cpu/cpu.build.zig");
const interrupt = @import("interrupt/interrupt.build.zig");
const mem = @import("mem/mem.build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: []const []const u8,
) void {

    kernel.root_module.addIncludePath(b.path("include"));

    // Set the architecture-dependent linker script
    kernel.setLinkerScript(b.path("x86.ld"));

    // Add these subsystems
    boot.addSubsystem(b, kernel, flags);
    cpu.addSubsystem(b, kernel, flags);
    interrupt.addSubsystem(b, kernel, flags);
    mem.addSubsystem(b, kernel, flags);
}
