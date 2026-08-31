const std = @import("std");
const helpers = @import("../../../../scripts/zig/helpers.zig");
const Flag = @import("../../../../shared.zig").Flag;

const Config = struct {
    multiboot: u32,
    multiboot2: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/arch/x86/boot";

    const config = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/boot.config.json", .{path})
    ) catch @panic("Failed to load boot.config.json");

    const c_sources = &[_][]const u8{
        "protocols/loader_parser.c",
    };

    const as_sources = &[_][]const u8{
        "entry.S",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
       b.fmt("-DMULTIBOOT_SUPPORT={d}", .{config.multiboot}),
       b.fmt("-DMULTIBOOT2_SUPPORT={d}", .{config.multiboot2}),
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

    // Multiboot2 source files
    if (config.multiboot2 == 1) {
        const multiboot2_c_sources = &[_][]const u8{
            "parsers/framebuffer.c",
            "parsers/generic.c",
            "parsers/mmap.c",
        };

        const multiboot2_as_sources = &[_][]const u8{
            "header.S",
        };

        kernel.root_module.addCSourceFiles(.{
            .root = b.path(b.fmt("{s}/protocols/multiboot2", .{path})),
            .files = multiboot2_c_sources,
            .flags = cflags,
        });

        kernel.root_module.addCSourceFiles(.{
            .root = b.path(b.fmt("{s}/protocols/multiboot2", .{path})),
            .files = multiboot2_as_sources,
            .flags = asflags,
        });
    }
}
