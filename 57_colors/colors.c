
#include <ctype.h>
#include "colors.h"

int hex_to_dec(char d) {
    if (isdigit(d)) {
        return d - '0';
    } else if ('a' <= d && d <= 'f') {
        return 10 + d - 'a';
    } else if ('A' <= d && d <= 'F') {
        return 10 + d - 'A';
    }
}

void string_to_color (struct color * col, const char * color_hex) {
    char color_canonical[] = "000000";
    for (int i = 0; ishexnumber(color_hex[i]) && i < 6; i++) {
        color_canonical[i] = color_hex[i];
    }
    col->red = 16 * hex_to_dec(color_canonical[0]) + hex_to_dec(color_canonical[1]);
    col->green = 16 * hex_to_dec(color_canonical[2]) + hex_to_dec(color_canonical[3]);
    col->blue = 16 * hex_to_dec(color_canonical[4]) + hex_to_dec(color_canonical[5]);
}
