const std = @import("std");
const Flag = @import("../../shared.zig").Flag;

const alloc = @import("alloc/alloc.build.zig");
const pmm = @import("pmm/pmm.build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {

    // Invoke these subsystems
    alloc.addSubsystem(b, kernel, flags);
    pmm.addSubsystem(b, kernel, flags);
}
