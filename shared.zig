const std = @import("std");

// Flags type
pub const Flag = struct {
    asflags: []const []const u8,
    cflags: []const []const u8,
};
