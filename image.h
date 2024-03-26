#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
    int width, height;
    uint32_t* pixels; // uint32_t* to represent RGBA pixels
} Image;

Image load_image(const char* filename);
void free_image(Image* img);

#endif // IMAGE_H
