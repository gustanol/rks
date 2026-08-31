const std = @import("std");
const Flag = @import("../../../shared.zig").Flag;

const boot = @import("boot/boot.build.zig");
const cpu = @import("cpu/cpu.build.zig");
const interrupt = @import("interrupt/interrupt.build.zig");
const mem = @import("mem/mem.build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/arch/x86";

    // This architecture include path
    kernel.root_module.addIncludePath(
        b.path(b.fmt("{s}/include", .{path}))
    );

    // Linker script
    kernel.setLinkerScript(
        b.path(b.fmt("{s}/x86.ld", .{path}))
    );

    boot.addSubsystem(b, kernel, flags);
    cpu.addSubsystem(b, kernel, flags);
    interrupt.addSubsystem(b, kernel, flags);
    mem.addSubsystem(b, kernel, flags);
}
