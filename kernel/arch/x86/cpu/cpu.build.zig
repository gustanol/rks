const std = @import("std");
const helpers = @import("../../../../scripts/zig/helpers.zig");

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    global_flags: []const []const u8,
) void {
    const path = "kernel/arch/x86/cpu";

    // Main sources
    const core_sources = &[_][]const u8{
      b.fmt("{s}/cpu_preflight.S", .{path}),
      b.fmt("{s}/descriptors/descriptors.S", .{path}),
      b.fmt("{s}/descriptors/gdt/boot_gdt.S", .{path}),
      b.fmt("{s}/descriptors/gdt/long_gdt.c", .{path}),
      b.fmt("{s}/descriptors/idt.c", .{path}),
      b.fmt("{s}/descriptors/tss.c", .{path}),
      b.fmt("{s}/transition_long.S", .{path}),
    };

    // Internal flags
    const cpu_cflags = helpers.flags.mergeFlags(b, global_flags, &.{
       "-I", b.fmt("{s}/include", .{path}),
    });

    // Add them to executable
    kernel.root_module.addCSourceFiles(.{
       .files = core_sources,
       .flags = cpu_cflags,
    });
}
