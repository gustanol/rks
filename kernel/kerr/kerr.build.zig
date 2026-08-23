const std = @import("std");
const helpers = @import("../../scripts/zig/helpers.zig");
const Flag = @import("../../shared.zig").Flag;

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/kerr";

    // Core source files
    const c_sources = &[_][]const u8{
        "kerr.c",
    };

    // Internal CFLAGS
    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{path}),
    });

    // Add to kernel global executable
    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = c_sources,
        .flags = cflags
    });
}
