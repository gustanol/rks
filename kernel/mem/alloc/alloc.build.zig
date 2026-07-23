const std = @import("std");

pub const Config = struct {
    heap: u32,
    slab: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/alloc";

    // Load local alloc.config.json
    const config = loadConfig(b, path);

    // ALLOC core soure files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/alloc.c", .{path}),
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // ALLOC internal CFLAGS
    const alloc_flags = mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("ALLOC_ENABLE_HEAP={d}", .{config.heap}),
        "-D", b.fmt("ALLOC_ENABLE_SLAB={d}", .{config.slab}),
    });

    // Add these files
    kernel.addCSourceFiles(.{
        .files = core_sources,
        .flags = alloc_flags,
    });

    // Heap allocator
    if (config.heap) {
        kernel.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/allocators/heap/heap.c", .{path})),
            .flags = alloc_flags,
        });
    }
}

fn loadConfig(b: *std.Build, path: []const u8) Config {
    const local_path = b.fmt("{s}/alloc.config.json", .{path});
    const default_path = b.fmt("{s}/alloc.config.default.json", .{path});

    const file = b.build_root.handle.openFile(local_path, .{}) catch
        b.build_root.handle.openFile(default_path, .{}) catch return .{};
    defer file.close();

    const contents = file.readToEndAlloc(b.allocator, 1024 * 64) catch return .{};
    const parsed = std.json.parseFromSlice(Config, b.allocator, contents, .{}) catch return .{};
    return parsed.value;
}

fn mergeFlags(
    b: *std.Build,
    global_flags: []const []const u8,
    extra_flags: []const []const u8,
) []const []const u8 {

    return std.mem.concat(
        b.allocator,
        []const u8,
        &.{ global_flags, extra_flags },

        // Out Of Memory error
    ) catch @panic("OOM merging build flags");
}
