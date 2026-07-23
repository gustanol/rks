const std = @import("std");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/mem/vmm";

    // VMM core source files
    const core_sources = &[_][]const u8{
        b.fmt("{s}/core/dispatch.c", .{path}),
        b.fmt("{s}/core/pmm.c", .{path}),
        b.fmt("{s}/core/wrappers.c", .{path}),
    };

    // VMM internal CFLAGS
    const vmm_flags = mergeFlags(b, global_flags, &.{
        "-I", b.fmt("{s}/include", .{path}),
    });

    // Add to kernel global executable
    kernel.addCSourceFiles(.{
        .files = core_sources,
        .flags = vmm_flags
    });
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
