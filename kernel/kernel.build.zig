const std = @import("std");

const arch = @import("arch/arch.build.zig");
const console = @import("console/console.build.zig");
const err = @import("err/err.build.zig");
const init = @import("init/init.build.zig");
const mem = @import("mem/mem.build.zig");

pub fn addKernel(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {

    // Top-level include folder
    kernel.addIncludePath(b.path("include"));

    // Invoke subsystems
    console.addSubsystem(b, kernel, global_flags);
    err.addSubsystem(b, kernel, global_flags);
    init.addSubsystem(b, kernel, global_flags);

    // Aggregator
    mem.addSubsystem(b, kernel, global_flags);
}
