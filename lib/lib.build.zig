const std = @import("std");

pub fn addLibrary(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
) void {
    const path = "lib";

    const klib = b.addStaticLibrary(.{
        .name = "klib",
        .root_source_file = b.path(b.fmt("{s}/main.zig", .{path})),
        .target = target,
        .optimize = optimize,
    });

    kernel.linkLibrary(klib);
}
