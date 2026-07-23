const std = @import("std");

pub const Config = struct {
    bitmap: u32,
    buddy: u32,
};

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/pmm";

    // Load local pmm.config.json (or default)
    const config = loadConfig(b, path);

    // PMM core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/pmm.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // PMM internal CFLAGS
    const pmm_flags = mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
        "-D", b.fmt("PMM_ENABLE_BITMAP={d}", .{config.bitmap}),
        "-D", b.fmt("PMM_ENABLE_BUDDY={d}", .{config.buddy}),
    });

    // Add to kernel global executable
    kernel.addCSourceFiles(.{
        .files = core_sources,
        .flags = pmm_flags
    });

    // Bitmap backend sources
    if (config.bitmap) {
        kernel.addCSourceFile(.{
            .file = b.path(b.fmt("{s}/backend/bitmap/bitmap.c", .{path})),
            .flags = pmm_flags,
        });
    }
}

fn loadConfig(b: *std.Build, path: []const u8) Config {
    const local_path = b.fmt("{s}/pmm.config.json", .{path});
    const default_path = b.fmt("{s}/pmm.config.default.json", .{path});

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
