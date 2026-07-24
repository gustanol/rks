const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/vmm";

    // VMM core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/vmm.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // VMM internal CFLAGS
    const vmm_flags = helpers.flags.mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
    });

    // Add to kernel global executable
    kernel.root_module.addCSourceFiles(.{
        .files = core_sources,
        .flags = vmm_flags
    });
}
