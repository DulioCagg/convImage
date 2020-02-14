#ifndef CONVIMAGE_H
#define CONVIMAGE_H
#include <stdlib.h>

void convImage(char *original, char *res);
unsigned char* applyMask(unsigned char* input, size_t input_width, size_t input_height, size_t input_channels, int* mask, size_t mask_size);
unsigned char* newImage(size_t input_width, size_t input_height, size_t input_channels);
unsigned char* imageToGrayscale(unsigned char* input, size_t input_width, size_t input_height, size_t input_channels, size_t* output_size, size_t* output_channels);

#endif