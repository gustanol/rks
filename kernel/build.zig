const std = @import("std");

const arch = @import("arch/build.zig");
const console = @import("console/build.zig");
const err = @import("err/build.zig");
const init = @import("init/build.zig");
const mem = @import("mem/build.zig");

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
