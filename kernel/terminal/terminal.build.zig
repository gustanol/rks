const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");

pub const Config = struct {
    vga: u32 = 1,
    framebuffer: u32 = 1,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/terminal";

    // Load local terminal.config.json (or default)
    const config_file = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/terminal.config.json", .{path})
    );

    // Terminal core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/init.c", .{path}),
        b.fmt("{s}/core/manager.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // Terminal internal CFLAGS
    const terminal_flags = helpers.flags.mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("TERMINAL_VGA_SUPPORT={d}", .{config_file.vga}),
        "-D", b.fmt("TERMINAL_FRAMEBUFFER_SUPPORT={d}", .{config_file.framebuffer}),
    });

    // Add to kernel global executable
    kernel.root_module.addCSourceFiles(.{
        .files = core_sources,
        .flags = terminal_flags
    });

    // VGA backend sources
    if (config_file.vga == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/backend/vga/vga.c", .{path})),
            .flags = terminal_flags,
        });
    }
}
