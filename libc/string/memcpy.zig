const std = @import("std");

pub export fn
memcpy(dest: [*]u8, src: [*]const u8, n: usize) callconv(.C) [*]u8
{

    // Current index
    var i: usize = 0;

    // Iterates from zero to `n` limit
    while (i < n) : ( i += 1 ) {

        // For each iteration, copy what's on `src` to `dest`
        dest[i] = src[i];
    }

    // Return `dest` array again
    return dest;
 }
