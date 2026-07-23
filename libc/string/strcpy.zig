const std = @import("std");
const strlen = @import("strlen.zig");

pub export fn
strcpy(dest: [*]u8, src: [*]const u8) callconv(.C) [*]u8
{

    // Gets `src` string size
    const n: usize = strlen.strlen(src);

    // Current size
    var i: usize = 0;

    // For each iteration, make copies from `src` to `dest`
    while (i < n) : (i += 1) {
        dest[i] = src[i];
    }

    // Return `dest` back
    return dest;
}

pub export fn
strncpy(dest: [*]u8, src: [*]const u8, count: usize) callconv(.C) [*]u8
{

    // `src` string length
    const n: usize = strlen.strlen(src);

    // Gets the minimum value between `count` and `n` (string length)
    const min: usize = if (n < count) n else count;

    // Current index
    var i: usize = 0;

    // For the first iteration, `src[i]` must be copied to `dest[i]`
    // Until the minimum size
    //
    // If `min = n`: String boundary. The rest will be filled up with `\0`
    // If `min = count`: The maximum allowed copy size. `dest` won't have `\0`
    while (i < min) : (i += 1) {
        dest[i] = src[i];
    }

    // Only if `min = count`
    if (i < count) {

        // Fills up with `\0`
        while (i < count) : (i += 1) {
            dest[i] = 0;
        }
    }

    // Return back
    return dest;
}
