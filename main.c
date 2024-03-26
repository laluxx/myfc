#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memset
#include "image.h"
#include "theme.h"

// TODO Use theme colors
// TODO Zoom in
// TODO Undestand why the bg clearing looks bad

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define START_POS_X 10
#define START_POS_Y 190
#define FONT_WIDTH 5
#define FONT_HEIGHT 6
#define CHAR_SPACING 1
#define CURSOR_WIDTH 5
#define CURSOR_HEIGHT 7
#define CURSOR_COLOR 0xFF0000FF
#define TEXT_COLOR 0xFFFFFFFF

Image fontImage;
int cursorPosX = START_POS_X;
int cursorPosY = START_POS_Y;
char textBuffer[1024] = {0};
int textLength = 0;
uint32_t framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z && textLength < sizeof(textBuffer) - 1) {
            char addedChar = key - GLFW_KEY_A + 'a';
            textBuffer[textLength++] = addedChar;
            cursorPosX += FONT_WIDTH + CHAR_SPACING;
        }
    }
}

void clear_framebuffer() {
    memset(framebuffer, 0, sizeof(framebuffer)); // Clearing the framebuffer
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

void draw_character(char c, int x, int y) {
    if (c < 'a' || c > 'z') return;
    int charIndex = c - 'a';
    int offsetX = charIndex * FONT_WIDTH; // Each character is 5 pixels wide

    // Loop through each pixel in the character's 5x6 grid
    for (int py = 0; py < FONT_HEIGHT; py++) {
        for (int px = 0; px < FONT_WIDTH; px++) {
            // Calculate the pixel's index in the fontImage's pixel array
            int pixelIndex = ((py * fontImage.width) + offsetX + px);

            // Get the color from the font image's pixels. Assuming the pixels are stored as RGBA
            uint32_t color = fontImage.pixels[pixelIndex];
            // Since the background is black, only draw pixels that are not black
            if (color != 0xFF000000) { // Assuming the black color is represented as 0xFF000000 in ARGB format
                draw_pixel(x + px, y + py, TEXT_COLOR);
            }
        }
    }
}


void draw_cursor() {
    for (int y = 0; y < CURSOR_HEIGHT; y++) {
        for (int x = 0; x < CURSOR_WIDTH; x++) {
            draw_pixel(cursorPosX + x, cursorPosY + y, CURSOR_COLOR);
        }
    }
}

void blit_framebuffer(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, framebuffer);
    glfwSwapBuffers(window);
}

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Fantasy Console", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetKeyCallback(window, key_callback);

    // Adjust OpenGL context for direct pixel manipulation
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    fontImage = load_image("./fontc.bmp");
    if (fontImage.pixels == NULL) {
        fprintf(stderr, "Failed to load font image\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(window)) {
        clear_framebuffer();

        // Draw the text from the buffer
        int x = START_POS_X, y = START_POS_Y;
        for (int i = 0; i < textLength; i++) {
            draw_character(textBuffer[i], x, y);
            x += FONT_WIDTH + CHAR_SPACING;
            if (x + FONT_WIDTH > SCREEN_WIDTH) {
                x = START_POS_X; // Reset to start of next line
                y += FONT_HEIGHT + CHAR_SPACING;
            }
        }

        draw_cursor();
        glClearColor(1, 1, 1, 1);
        blit_framebuffer(window);

        glfwPollEvents();
    }

    free_image(&fontImage);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
