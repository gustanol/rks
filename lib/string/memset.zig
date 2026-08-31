const std = @import("std");

pub export fn
memset(dest: [*]u8, c: u8, n: usize) callconv(.C) [*]u8
{

    // Current index
    var i: usize = 0;

    // Iterates up to array limit
    while (i < n) : (i += 1) {

        // For each index, set it the given char
        dest[i] = c;
    }

    // Return the array back
    return dest;
}
