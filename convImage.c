#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(void) {
  int width, height, channels;
  unsigned char *img = stbi_load("img/image.png", &width, &height, &channels, 0);
  //int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  int mask[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

  
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

  size_t result_img_size = width * height * gray_channels;
  unsigned char *result_img = malloc(result_img_size);

  // Applies the convolution to the image
  int index = 0;
  for(unsigned char *p = gray_img, *pr = result_img; p != gray_img + gray_img_size; p += gray_channels, pr += gray_channels) {
    size_t y = index / height;
    size_t x = index % width;
    uint8_t acc = 0;

    for(size_t i = 0; i < sizeof mask; i++) {
      int mask_dim = sqrt(sizeof mask);

      if (x == 0 && y == 0) {
        
      } else if (x == 0 && y == height - 1) {
        
      } else if (x == width - 1 && y == 0) {

      } else if (x == width - 1 && y == height - 1) {

      } else if (x == 0) {

      } else if (y == 0) {

      } else if (x == width - 1) {

      } else if (y == height - 1) {
        
      } else {
        size_t dy = (i / mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));
        size_t dx = (i % mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));
        //printf("DY: %ld\n", dy);
        //printf("DX: %ld\n", dx);
        //printf("Pizel: %d\n", (uint8_t)(*p + dx + (dy * width)));
        //printf("Mask[i]: %d\n", mask[i]);

        acc += (uint8_t)(*p + dx + (dy * width)) * mask[i];
      }
    }
    *pr = acc;
    acc = 0;
    index++;
  }

  // Save image and free memory space
  stbi_write_jpg("img/gray.jpg", width, height, gray_channels, gray_img, 100);
  stbi_image_free(img);

  stbi_write_jpg("img/result.jpg", width, height, gray_channels, result_img, 100);
}