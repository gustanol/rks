const std = @import("std");
const helpers = @import("../../scripts/zig/helpers.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/init";

    // Core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/main.c", .{path}),
        b.fmt("{s}/subsys/registry.c", .{path}),
    };

    // Internal CFLAGS
    const pmm_flags = helpers.flags.mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
    });

    // Add to kernel global executable
    kernel.root_module.addCSourceFiles(.{
        .files = core_sources,
        .flags = pmm_flags
    });
}
