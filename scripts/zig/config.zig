const std = @import("std");

pub fn loadLocalConfigFile(comptime T: type, b: *std.Build, path: []const u8) T {
    const io = b.graph.io;

    // Read all file content
    const contents = b.build_root.handle.readFileAlloc(
        io, path, b.allocator,
        std.Io.Limit.unlimited
    ) catch return .{};

    // Parse it according the given type
    const parsed = std.json.parseFromSlice(
        T, b.allocator, contents, .{}
    ) catch return .{};

    return parsed.value;
}
