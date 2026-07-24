const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");

pub const Config = struct {
    bitmap: u32 = 1,
    buddy: u32 = 1,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/pmm";

    // Load local pmm.config.json (or default)
    const config_file = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/pmm.config.json", .{path})
    );

    // PMM core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/pmm.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // PMM internal CFLAGS
    const pmm_flags = helpers.flags.mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("PMM_ENABLE_BITMAP={d}", .{config_file.bitmap}),
        "-D", b.fmt("PMM_ENABLE_BUDDY={d}", .{config_file.buddy}),
    });

    // Add to kernel global executable
    kernel.root_module.addCSourceFiles(.{
        .files = core_sources,
        .flags = pmm_flags
    });

    // Bitmap backend sources
    if (config_file.bitmap == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/backend/bitmap/bitmap.c", .{path})),
            .flags = pmm_flags,
        });
    }
}
