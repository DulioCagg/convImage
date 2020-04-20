#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"



// Allocates the space in memory and returns the pointer of that space
unsigned char* newImage(size_t input_width, size_t input_height, size_t input_channels) {

    size_t output_size = input_width * input_height * input_channels;

    // Allocates the memory for managing the new image
    unsigned char* output_img = (unsigned char*)malloc(output_size);
    if (output_img == NULL) {
        printf("\nError:\tUnable to allocate memory for the gray image\n");
        return 0;
    }

    return output_img;
}

// Transforms an image to gray-scale taking the mean between the values of the RGB values
unsigned char* imageToGrayscale(unsigned char* input,
                 size_t         input_width,
                 size_t         input_height,
                 size_t         input_channels,
                 size_t*        output_size,
                 size_t*        output_channels) {
    size_t input_size = input_width * input_height * input_channels;
    // Convert the image to gray-scale, if the image contains a transparency value, it has 2 channels
    int            grey_channels = input_channels == 4 ? 2 : 1;
    size_t         grey_img_size = input_width * input_height * grey_channels;
    unsigned char* gray_img      = newImage(input_width, input_height, grey_channels);

    // P for original, PG for new gray image
    for (unsigned char *p = input, *pg = gray_img; p != input + input_size; p += input_channels, pg += grey_channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
        if (input_channels == 4) {
            *(pg + 1) = *(p + 3);
        }
    }

    *output_size     = grey_img_size;
    *output_channels = grey_channels;

    return gray_img;
}

// Applies the convolution to the image
unsigned char* applyMask(unsigned char* input, size_t input_width, size_t input_height, size_t input_channels, int* mask, size_t mask_size, int* argc, char **argv) {
    unsigned char* output     = newImage(input_width, input_height, input_channels);
    size_t         input_size = input_width * input_height * input_channels;

    int mpi_size, mpi_rank;

    MPI_Init(argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    // MPI_Send( &buffer, count, datatype, destination, tag, comm)
    // MPI_Recv( &buffer, count, datatype, source, tag, comm, &status)

    // Sends individual messages from the root process to all other processes
    // MPI_Scatter( $sendBuffer, sendcount, sendtype, &recoveryBuffer, recoverycount, recoveryType, root, comm)

    // MPI_Gather(&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm)

    int index = 0;
    for (unsigned char *p = input, *pr = output; p != input + input_size; p += input_channels, pr += input_channels) {
        size_t y   = index / input_height;
        size_t x   = index % input_width;
        int    acc = 0;

        for (int i = 0; i < mask_size; i++) {
            int mask_dim = sqrt(mask_size);

            if (x == 0 && y == 0) {

            } else if (x == 0 && y == input_height - 1) {

            } else if (x == input_width - 1 && y == 0) {

            } else if (x == input_width - 1 && y == input_height - 1) {

            } else if (x == 0) {

            } else if (y == 0) {

            } else if (x == input_width - 1) {

            } else if (y == input_height - 1) {

            } else {
                size_t dy = (i / mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));
                size_t dx = (i % mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));

                uint32_t value = ((uint32_t) * (p + dx + (dy * input_width))) * mask[i];

                acc += value;
            }
        }

        // Delimits the value for the pixel to black or white for edge detection
        if (acc < 0) {
            acc = 0;
        } else if (acc > 255) {
            acc = 255;
        }
        *pr = (unsigned char)acc;
        index++;
    }

    MPI_Finalize();

    return output;
}





