const std = @import("std");
const helpers = @import("../../../../scripts/zig/helpers.zig");

pub const Config = struct {
    multiboot: u32,
    multiboot2: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/arch/x86/boot";

    // Configuration file loading
    const config_file = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/boot.config.json", .{path})
    ) catch @panic("Failed to load boot.config.json");

    // Boot specific CFLAGS
    const boot_cflags = helpers.flags.mergeFlags(b, global_flags, &.{
       "-D", b.fmt("MULTIBOOT_SUPPORT={d}", .{config_file.multiboot}),
       "-D", b.fmt("MULTIBOOT2_SUPPORT={d}", .{config_file.multiboot2}),
    });

    // Entry file
    kernel.root_module.addAssemblyFile(.{
        .file = b.path(b.fmt("{s}/entry.S", .{path})),
        .flags = boot_cflags,
    });

    // Boot protocol parser
    kernel.root_module.addCSourceFile(.{
        .file = b.path(b.fmt("{s}/protocols/loader_parser.c", .{path})),
        .flags = boot_cflags,
    });

    // Multiboot2 source files
    if (config_file.multiboot2 == 1) {
        kernel.root_module.addAssemblyFile(.{
            .file = b.path(b.fmt("{s}/protocols/multiboot2/header.S", .{path})),
            .flags = boot_cflags,
        });

        kernel.root_module.addCSourceFiles(.{
           .files = &[_][]const u8{
               b.fmt("{s}/protocols/multiboot2/parsers/framebuffer.c", .{path}),
               b.fmt("{s}/protocols/multiboot2/parsers/generic.c", .{path}),
               b.fmt("{s}/protocols/multiboot2/parsers/mmap.c", .{path}),
           }
        });
    }
}
