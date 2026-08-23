const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");
const Flag = @import("../../../shared.zig").Flag;

const Config = struct {
    bitmap: u32,
    buddy: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/mem/pmm";

    const config = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/pmm.config.json", .{path})
    ) catch @panic("Failed to load pmm.config.json");

    const c_sources = &[_][]const u8{
        "core/dispatch.c",
        "core/pmm.c",
        "core/wrappers.c",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("PMM_ENABLE_BITMAP={d}", .{config.bitmap}),
        "-D", b.fmt("PMM_ENABLE_BUDDY={d}", .{config.buddy}),
    });

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = c_sources,
        .flags = cflags,
    });

    // Bitmap backend sources
    if (config.bitmap == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/backend/bitmap/bitmap.c", .{path})),
            .flags = cflags,
        });
    }
}
