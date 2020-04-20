#include "convImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Declares the Kernel for the convolution with its size
int size_mask = 9;
int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};



// argc = argument count, argv = pointer to charstring containing argument
int main(int argc, char* argv[]) {

    char inputPath[3][100]; 
    char resultPath[3][100];

    size_t image_count = 3;

    // Define image paths
    strcpy(inputPath[0], "img/originals/imageSmall.jpg");
    strcpy(resultPath[0], "img/results/MPI/imageSmall.jpg");
    strcpy(inputPath[1], "img/originals/imageNormal.jpg");
    strcpy(resultPath[1], "img/results/MPI/imageNormal.jpg");
    strcpy(inputPath[2], "img/originals/imageBig.jpg");
    strcpy(resultPath[2], "img/results/MPI/imageBig.jpg");


    // // Argument Parsing
    // if (argc > 2) {
    //     if (argc > 2) strcpy(resultPath, argv[2]);   // resultPath = *argv[2];
    //     if (argc > 1) strcpy(inputPath, argv[1]);   // inputPath = *argv[1];

    // } else printf("Image Convolution in C\n\tArguments:\n\t\tinputPath outputPath\n");

    // Apply convolution to each image
    for (size_t image = 0; image < image_count; image++) {

        // Load the input image
        int width, height, channels;
        const char *image_source = inputPath[image];
        const char *output_location = resultPath[image];

        unsigned char* img = stbi_load(image_source, &width, &height, &channels, 0);

        if (img == NULL) {
            printf("Error loading the image\n");
            exit(1);
        }


        // Convert the image to gray-scale, if the image contains a transparency value, it has 2 channels
        size_t         img_size = width * height * channels;
        size_t         gray_img_size, gray_channels;
        unsigned char* gray_img = imageToGrayscale(img, width, height, channels, &gray_img_size, &gray_channels);

        // Takes the time it takes to apply the mask to the image
        clock_t        startTime   = clock();
        unsigned char* result_img  = applyMask(gray_img, width, height, gray_channels, mask, size_mask, &argc, argv);
        clock_t        elapsedTime = clock() - startTime;

        // Calculate time taken and output
        double seconds = ((double)elapsedTime) / CLOCKS_PER_SEC;
        // printf("Convolution for %s took: %.3f seconds.\n", inputPath, seconds);
        printf("%.4f", seconds);

        
        stbi_image_free(gray_img);
        stbi_write_jpg(output_location, width, height, gray_channels, result_img, 100);
        stbi_image_free(result_img);
    }
    


    return 0;
}

