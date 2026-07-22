const std = @import("std");

const alloc = @import("alloc/build.zig");
const pmm = @import("pmm/build.zig");
const vmm = @import("vmm/build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: []const []const u8,
) void {

    // Invoke these subsystems
    alloc.addSubsystem(b, kernel, flags);
    pmm.addSubsystem(b, kernel, flags);
    vmm.addSubsystem(b, kernel, flags);
}
