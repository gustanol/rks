const std = @import("std");
const helpers = @import("../../../../../../scripts/zig/helpers.zig");
const Flag = @import("../../../../../../shared.zig").Flag;

pub fn addSubsystem(
    b: *std.Build,
    kernel: *std.Build.Step.Compile,
    flags: Flag
) void {
    const paging_path = "kernel/arch/x86_64/mem/vmm/paging";
    const vmm_path = "kernel/mem/vmm";

    const vmm_c_sources = &[_][]const u8{
        "core/dispatch.c",
        "core/vmm.c",
        "core/wrappers.c",
    };

    const paging_c_sources = &[_][]const u8{
        "early_map.c",
        "init.c",
        "page.c",
        "space.c",
    };

    const vmm_cflags = helpers.flags.mergeFlags(b, flags.cflags, &.{
        "-I", b.fmt("{s}/include", .{vmm_path}),
    });

    // Paging-only flags (add here if needed)
    const paging_cflags = vmm_cflags;

    kernel.root_module.addCSourceFiles(.{
       .root = b.path(vmm_path),
       .files = vmm_c_sources,
       .flags = vmm_cflags,
    });

    kernel.root_module.addCSourceFiles(.{
       .root = b.path(paging_path),
       .files = paging_c_sources,
       .flags = paging_cflags,
    });
}
