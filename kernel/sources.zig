const std = @import("std");

const arch = @import("arch/sources.zig");
const console = @import("console/sources.zig");
const err = @import("err/sources.zig");
const init = @import("init/sources.zig");
const mem = @import("mem/sources.zig");

pub fn addKernel(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: []const []const u8,
) void {

    // Top-level include folder
    kernel.addIncludePath(b.path("include"));

    // Invoke subsystems
    console.addSubsystem(b, kernel, flags);
    err.addSubsystem(b, kernel, flags);
    init.addSubsystem(b, kernel, flags);

    // Aggregator
    mem.addSubsystem(b, kernel, flags);
}
