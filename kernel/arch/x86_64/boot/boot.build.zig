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
    const kernel_path = "kernel/boot";
    const arch_path = "kernel/arch/x86_64/boot";

    const config = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/boot.config.json", .{arch_path})
    ) catch @panic("Failed to load boot.config.json");

    const c_sources = &[_][]const u8{
        "generic_parser.c",
    };

    const as_sources = &[_][]const u8{
        "entry.S",
        "header.S",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
       b.fmt("-DMULTIBOOT_SUPPORT={d}", .{config.multiboot}),
       b.fmt("-DMULTIBOOT2_SUPPORT={d}", .{config.multiboot2}),
       b.fmt("-I{s}/include", .{kernel_path}),
    });

    const asflags = helpers.flags.mergeFlags(b, cflags, flags.asflags);

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(kernel_path),
        .files = c_sources,
        .flags = cflags,
    });

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(arch_path),
        .files = as_sources,
        .flags = asflags,
    });

    // Multiboot2 support
    if (config.multiboot2 == 1) {
        const multiboot2_c_sources = &[_][]const u8{
            "protocols/multiboot2.c",
        };

        kernel.root_module.addCSourceFiles(.{
            .root = b.path(b.fmt("{s}", .{kernel_path})),
            .files = multiboot2_c_sources,
            .flags = cflags,
        });
    }
}
