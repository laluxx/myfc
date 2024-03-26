#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image.h"
#include <stdlib.h>

/*
  Bitmap file format
  SECTION
  Address:Bytes	Name
  
  HEADER:
 	  0:	2		"BM" magic number
 	  2:	4		file size
 	  6:	4		junk
 	 10:	4		Starting address of image data
   BITMAP HEADER:
 	 14:	4		header size
 	 18:	4		width  (signed)
 	 22:	4		height (signed)
 	 26:	2		Number of color planes
 	 28:	2		Bits per pixel
 	[...]
   [OPTIONAL COLOR PALETTE, NOT PRESENT IN 32 BIT BITMAPS]
   BITMAP DATA:
 	DATA:	X	Pixels  
*/


Image load_image(const char* filename) {
    Image img = {0};
    int comp; // Will hold the number of channels in the image

    // Use stbi_load to load the image's data
    unsigned char* data = stbi_load(filename, &img.width, &img.height, &comp, 4); // Force RGBA format

    if (data) {
        size_t dataSize = img.width * img.height * 4; // 4 bytes per pixel (RGBA)
        img.pixels = (uint32_t*)malloc(dataSize);
        if (img.pixels) {
            // Flip the image vertically as we copy it
            for (int y = 0; y < img.height; y++) {
                for (int x = 0; x < img.width; x++) {
                    int srcIndex = (y * img.width + x) * 4;
                    int dstIndex = ((img.height - 1 - y) * img.width + x) * 4;
                    uint32_t r = data[srcIndex];
                    uint32_t g = data[srcIndex + 1];
                    uint32_t b = data[srcIndex + 2];
                    uint32_t a = data[srcIndex + 3];
                    img.pixels[dstIndex / 4] = (a << 24) | (b << 16) | (g << 8) | r; // Convert to uint32_t and ensure correct order
                }
            }
        }
        stbi_image_free(data); // Free the original data
    } else {
        // Handle error
    }

    return img;
}

void free_image(Image* img) {
    if (img && img->pixels) {
        free(img->pixels);
        img->pixels = NULL;
    }
    img->width = img->height = 0;
}

