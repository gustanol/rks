const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");
const Flag = @import("../../../shared.zig").Flag;

const Config = struct {
    heap: u32,
    slab: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag,
) void {
    const path = "kernel/mem/alloc";

    // Load local alloc.config.json
    const config = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/alloc.config.json", .{path})
    ) catch @panic("Failed to load alloc.config.json");

    const c_sources = &[_][]const u8{
        "core/alloc.c",
        "core/dispatch.c",
        "core/wrappers.c",
    };

    const cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        b.fmt("-DALLOC_ENABLE_HEAP={d}", .{config.heap}),
        b.fmt("-DALLOC_ENABLE_SLAB={d}", .{config.slab}),
    });

    kernel.root_module.addCSourceFiles(.{
        .root = b.path(path),
        .files = c_sources,
        .flags = cflags,
    });

    // Heap allocator
    if (config.heap == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/allocators/heap/heap.c", .{path})),
            .flags = cflags,
        });
    }
}
