#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool is_valid_hex(const char *hex) {
    if (strlen(hex) != 6) {
        return false;
    }
    for (int i = 0; i < 6; i++) {
        if (!isxdigit(hex[i])) {
            return false;
        }
    }
    return true;
}

void convert_hex_to_rgb(const char *hex, int *r, int *g, int *b) {
    sscanf(hex, "%2x%2x%2x", r, g, b);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *current = argv[i];
        if (is_valid_hex(current)) {
            int r, g, b;
            convert_hex_to_rgb(current, &r, &g, &b);
            printf("R%3d G%3d B%3d\n", r, g, b);
        } else {
            printf("Invalid hex color: %s\n", current);
        }
    }
    return 0;
}