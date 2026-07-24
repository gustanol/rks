const std = @import("std");
const helpers = @import("../../../scripts/zig/helpers.zig");

pub const Config = struct {
    heap: u32 = 1,
    slab: u32 = 1,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/alloc";

    // Load local alloc.config.json
    const config_file = helpers.config.loadLocalConfigFile(
        Config, b, b.fmt("{s}/alloc.config.json", .{path})
    );

    // ALLOC core soure files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/alloc.c", .{path}),
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // ALLOC internal CFLAGS
    const alloc_flags = helpers.flags.mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("ALLOC_ENABLE_HEAP={d}", .{config_file.heap}),
        "-D", b.fmt("ALLOC_ENABLE_SLAB={d}", .{config_file.slab}),
    });

    // Add these files
    kernel.root_module.addCSourceFiles(.{
        .files = core_sources,
        .flags = alloc_flags,
    });

    // Heap allocator
    if (config_file.heap == 1) {
        kernel.root_module.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/allocators/heap/heap.c", .{path})),
            .flags = alloc_flags,
        });
    }
}
