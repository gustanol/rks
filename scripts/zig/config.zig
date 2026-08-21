const std = @import("std");

pub fn loadLocalConfigFile(comptime T: type, b: *std.Build, path: []const u8) !T {
    
    const contents = try b.build_root.handle.readFileAlloc(
        b.allocator,
        path,
        10 * 1024 * 1024, // 10MiB
    );
    defer b.allocator.free(contents);

    const parsed = try std.json.parseFromSlice(
        T,
        b.allocator,
        contents,
        .{ .ignore_unknown_fields = true },
    );

    return parsed.value;
}
