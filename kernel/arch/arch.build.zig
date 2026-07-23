const std = @import("std");

// Every architecture build file
const x86 = @import("x86/build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: []const []const u8,
) void {

    // Gets the chosen architecture
    const arch = kernel.root_module.resolved_target.?.result.cpu.arch;

    switch (arch) {
        .x86 => x86.addSubsystem(b, kernel, flags),
        else => @panic("Unsupported target architecture"),
    }
}
