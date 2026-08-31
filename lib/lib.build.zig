const std = @import("std");

pub fn addLibrary(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
) void {
    const path = "libc";

    const libc = b.addStaticLibrary(.{
        .name = "c",
        .root_source_file = b.path(b.fmt("{s}/main.zig", .{path})),
        .target = target,
        .optimize = optimize,
    });

    kernel.linkLibrary(libc);
}
