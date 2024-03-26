#ifndef THEME_H
#define THEME_H

typedef struct {
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct {
    Color cursor;
    Color console_bg;
    Color console_text;
    Color editor_bg;
    Color editor_comment;
    Color editor_keyword;
    Color editor_number;
    Color editor_string;
} Theme;

extern Theme themes[];
extern int currentThemeIndex;
#define CT (themes[currentThemeIndex])

Color hex_to_color(const char *hex);
void nextTheme();
void initializeThemes();
void nextTheme();


#endif // THEME_H
