#include "convImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Declares the Kernel for the convolution with its size
int size_mask = 9;
int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};

// argc = argument count, argv = pointer to charstring containing argument
int main(int argc, char** argv) {
    // Arguments: InputImage, OutputImage
    char inputPath[100]  = "../img/originals/image"; // "img/originals/imageSmall.jpg";
    char resultPath[100] = "../img/results/OpenMP/image"; // "img/results/Sequential/resultsSmall.jpg";
    char name[50] = "";

        

    // Argument Parsing
    if (argc > 1) {
        strcpy(name, argv[1]);   // inputPath = *argv[1];
        strcat(inputPath, name);
        strcat(resultPath, name);
    } else printf("Image Convolution in C\n\tArguments:\n\t\tinputPath outputPath threadCount\n");

    
    // Load the input image
    int width, height, channels;
    unsigned char *img = stbi_load(inputPath, &width, &height, &channels, 1);

    if (img == NULL) {
        printf("Error loading the image\n");
        exit(1);
    }

    // File management
    FILE *fp;
    fp = fopen("results.txt", "a");
    fprintf(fp, "%s\n", name);

    // Convert the image to gray-scale, if the image contains a transparency value, it has 2 channels
    size_t img_size = width * height;

    // Takes the time it takes to apply the mask to the image
    for(int i = 1; i <= 10; i++) {
        fprintf(fp, "%d\n", i);
        printf("File: %s, epoch: %d\n", name, i);
        for(int threads = 2; threads < 9; threads++) {
            unsigned char* result_img = applyMask(img, width, height, 1, mask, size_mask, threads, fp);
            stbi_write_jpg(resultPath, width, height, 1, result_img, 100);
            stbi_image_free(result_img);
        }
    }
    fclose(fp);
    // Save image and free memory space
    stbi_image_free(img);

    return 0;
}

