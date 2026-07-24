const std = @import("std");

pub fn mergeFlags(
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
