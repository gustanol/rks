const std = @import("std");

const alloc = @import("alloc/alloc.build.zig");
const pmm = @import("pmm/pmm.build.zig");
const vmm = @import("vmm/vmm.build.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {

    // Invoke these subsystems
    alloc.addSubsystem(b, kernel, global_flags);
    pmm.addSubsystem(b, kernel, global_flags);
    vmm.addSubsystem(b, kernel, global_flags);
}
