const std = @import("std");
const Flag = @import("../shared.zig").Flag;

const kerr = @import("kerr/kerr.build.zig");
const init = @import("init/init.build.zig");
const mem = @import("mem/mem.build.zig");
const terminal = @import("terminal/terminal.build.zig");

pub fn addKernel(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {

    // Top-level include folder
    kernel.root_module.addIncludePath(b.path("include"));

    // Invoke subsystems
    kerr.addSubsystem(b, kernel, flags);
    init.addSubsystem(b, kernel, flags);
    terminal.addSubsystem(b, kernel, flags);
    mem.addSubsystem(b, kernel, flags);
}
