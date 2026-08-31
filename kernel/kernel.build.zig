const std = @import("std");
const Flag = @import("../shared.zig").Flag;

const arch = @import("arch/arch.build.zig");
const kerr = @import("kerr/kerr.build.zig");
const init = @import("init/init.build.zig");
const mem = @import("mem/mem.build.zig");
const terminal = @import("terminal/terminal.build.zig");

pub fn addKernel(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {

    // Invoke subsystems
    arch.addSubsystem(b, kernel, flags);
    init.addSubsystem(b, kernel, flags);
    mem.addSubsystem(b, kernel, flags);
    terminal.addSubsystem(b, kernel, flags);
    kerr.addSubsystem(b, kernel, flags);
}
