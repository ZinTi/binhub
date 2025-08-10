#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstdbool>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include "binhub.h"

#ifdef __cplusplus
}
#endif

#define PROGRAM_NAME        "BINHUB CvtColor"
#define AUTHOR_NAME         "Mr. ZENG Lai(Zin)"

#define DEBUG_LOG(msg) do { \
if (g_debug_enabled) { \
fprintf(stderr, "LOG: %s\n", msg); \
} \
} while (0)


typedef binhub_rgb_t RGB;


bool is_valid_color_hex(const char* hex);
void print_rgb(const RGB* rgb);

int main(int argc, char* argv[]) {
    /*
    const RGB* rgb1 = binhub_rgb_new(99, 127, 236);
    const RGB* rgb2 = binhub_rgb_new(27, 128, 20);
    RGB* rgb3 = binhub_rgb_new(0, 0, 0);
    binhub_rgb_add_saturate(rgb1, rgb2, rgb3);
    char str3[100];
    binhub_rgb_to_string(rgb3, RGB_FORMAT_CSV, str3, sizeof(str3));
    printf("%s\n", str3);
    */


    for (int i = 1; i < argc; i++) {
        char* current = argv[i];
        if (is_valid_color_hex(current)) {
            RGB* rgb = binhub_rgb_new(0, 0, 0);
            binhub_hex_to_rgb(current, rgb);
            print_rgb(rgb);
        } else {
            printf("Invalid hex color: %s\n", current);
        }
    }
    return 0;
}

bool is_valid_color_hex(const char* hex) {
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


void print_rgb(const RGB* rgb) {
    printf("RGB(");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_RED, CLR_NONE}, "%3d", binhub_rgb_get_red(rgb));
    printf(", ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_GREEN, CLR_NONE}, "%3d", binhub_rgb_get_green(rgb));
    printf(", ");
    binhub_cprintf((binhub_tcolor_t){CLR_BRT_BLUE, CLR_NONE}, "%3d", binhub_rgb_get_blue(rgb));
    printf(")\n");
}