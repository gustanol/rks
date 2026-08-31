const std = @import("std");
const helpers = @import("../../../../scripts/zig/helpers.zig");
const Flag = @import("../../../../shared.zig").Flag;

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/arch/x86_64/interrupt";

    const c_sources = &[_][]const u8{
        "handlers/generic.c",
        "handlers/page_fault.c",
    };

    const as_sources = &[_][]const u8{
        "isr.S",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{path}),
    });

    const asflags = helpers.flags.mergeFlags(b, cflags, flags.asflags);

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = c_sources,
        .flags = cflags,
    });

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = as_sources,
        .flags = asflags,
    });
}
