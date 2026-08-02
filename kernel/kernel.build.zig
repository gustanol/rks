const std = @import("std");

const kerr = @import("kerr/kerr.build.zig");
const init = @import("init/init.build.zig");
const mem = @import("mem/mem.build.zig");
const terminal = @import("terminal/terminal.build.zig");

pub fn addKernel(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {

    // Top-level include folder
    kernel.root_module.addIncludePath(b.path("include"));

    // Invoke subsystems
    kerr.addSubsystem(b, kernel, global_flags);
    init.addSubsystem(b, kernel, global_flags);
    terminal.addSubsystem(b, kernel, global_flags);

    // Aggregator
    mem.addSubsystem(b, kernel, global_flags);
}
