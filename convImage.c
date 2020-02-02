#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(void) {
  int width, height, channels;
  unsigned char *img = stbi_load("img/image.png", &width, &height, &channels, 0);
  
  if(img == NULL) {
    printf("Error loading the image\n");
    exit(1);
  }

  // Convert the image to gray-scale, if the image contains a transparency value, it has 2 channels
  size_t img_size = width * height * channels;
  int gray_channels = channels == 4 ? 2 : 1;
  size_t gray_img_size = width * height * gray_channels;

  // Allocates the memory for managing the new image
  unsigned char *gray_img = malloc(gray_img_size);
  if(gray_img == NULL) {
    printf("Unable to allocate memory for the gray image\n");
    exit(1);
  }

  // P for original, PG for new gray image
  for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
    *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    if(channels == 4) {
      *(pg + 1) = *(p + 3);
    }
  }

  // Save image and free memory space
  stbi_write_jpg("img/gray.jpg", width, height, gray_channels, gray_img, 100);
  stbi_image_free(img);
}