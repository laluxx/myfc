#include <stdio.h>
#include <stdlib.h>
#include "theme.h"

// TODO make this work for both #RRGGBB and #RRGGBBA
// if the alpha component is missing just assume its 100%
Color hex_to_color(const char* hex) {
    // Assuming hex is always a valid 6-character string for simplicity
    unsigned int int_value;
    Color color;

    // Convert hex string to an unsigned int
    sscanf(hex, "%x", &int_value);

    color.r = ((int_value >> 16) & 0xFF) / 255.0f;
    color.g = ((int_value >> 8) & 0xFF) / 255.0f;
    color.b = (int_value & 0xFF) / 255.0f;
    color.a = 1.0f; // Assuming fully opaque

    return color;
}

int currentThemeIndex = 0;
Theme themes[2];

void initializeThemes() {
    themes[0] = (Theme) {
        .cursor = hex_to_color("#FF77A8"),
        .console_bg = hex_to_color("#000000"),
        .console_text = hex_to_color("#FCF2EA"),
    };

    themes[1] = (Theme) {
        .cursor = hex_to_color("#FFFFFF"),
        .console_bg = hex_to_color("#000000"),
        .console_text = hex_to_color("#FCF2EA"),
    };

}

void nextTheme() {
    currentThemeIndex++;
    if (currentThemeIndex >= sizeof(themes) / sizeof(Theme)) {
        currentThemeIndex = 0;
    }
}

void previousTheme() {
    currentThemeIndex--;
    if (currentThemeIndex < 0) {
        currentThemeIndex = sizeof(themes) / sizeof(Theme) - 1;
    }
}
