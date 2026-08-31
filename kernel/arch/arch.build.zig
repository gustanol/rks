const std = @import("std");
const builtin = @import("builtin");
const Flag = @import("../../shared.zig").Flag;

// Every architecture build file
const x86_64 = @import("x86_64/x86_64.build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {

    // Gets the chosen architecture
    const arch = kernel.root_module.resolved_target.?.result.cpu.arch;

    switch (arch) {
        .x86_64 => x86_64.addSubsystem(b, kernel, flags),
        else => @panic("Unsupported target architecture"),
    }
}
