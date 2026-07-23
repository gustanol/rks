const std = @import("std");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
) void {
    const libc_dir = "libc";

    // Build the static library
    const klibc = b.addStaticLibrary(.{
        .name = "c", // Generates libc.a
        .root_module = b.createModule(.{
            .root_source_file = b.path(b.fmt("{s}/main.zig", .{libc_dir})),
            .target = target,
            .optimize = optimize,
        }),
    });

    // Link the Zig static library into the kernel binary
    kernel.linkLibrary(klibc);
}
