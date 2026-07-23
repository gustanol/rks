const std = @import("std");
const strlen = @import("strlen.zig");

pub export fn
strcat(dest: [*]u8, src: [*]const u8) callconv(.C) [*]u8
{

    // `dest` length
    const n = strlen.strlen(dest);

    // Current `src` index
    var i: usize = 0;

    while (true) : (i += 1) {

        // Copies `src[i]` to the final of `dest`
        dest[n + i] = src[i];

        // Stops on null terminator
        if (src[i] == 0)
            break;
    }

    // Return it back
    return dest;
}
