const std = @import("std");

pub export fn
strlen(src: [*]const u8) callconv(.C) usize
{

    // Current size
    var i: usize = 0;

    // If the current char is not a null terminator,
    // Increases the size
    while (src[i] != 0) : (i += 1) {}

    // Return it
    return i;
}

pub export fn
strlen_s(src: [*]const u8, strsz: usize) callconv(.C) usize
{
    var i: usize = 0;

    while (src[i] != 0) : (i += 1) {

        // If the null terminator weren't found 'til the limit,
        // Return the given size back
        if (i == strsz) return strsz;
    }

    // Return it
    return i;
}
