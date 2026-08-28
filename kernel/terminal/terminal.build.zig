const std = @import("std");
const helpers = @import("../../scripts/zig/helpers.zig");
const Flag = @import("../../shared.zig").Flag;

const Config = struct {
    vga: u32,
    framebuffer: u32,
    tty_lines: u32,
    tty_columns: u32
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/terminal";

    const config = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/terminal.config.json", .{path})
    ) catch @panic("Failed to load terminal.config.json");

    const c_sources = &[_][]const u8{
        "core/dispatch.c",
        "core/init.c",
        "core/manager.c",
        "core/wrappers.c",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        b.fmt("-DTERMINAL_VGA_SUPPORT={d}", .{config.vga}),
        b.fmt("-DTERMINAL_FRAMEBUFFER_SUPPORT={d}", .{config.framebuffer}),
        b.fmt("-DTTY_LINES={d}", .{config.tty_lines}),
        b.fmt("-DTTY_COLUMNS={d}", .{config.tty_columns}),
    });

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = c_sources,
        .flags = cflags,
    });

    // VGA backend sources
    if (config.vga == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/backend/vga/vga.c", .{path})),
            .flags = cflags,
        });
    }
}
