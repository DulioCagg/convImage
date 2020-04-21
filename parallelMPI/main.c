#include "convImage.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MPI_Status status;
unsigned char *img;

void rebuild_img(unsigned char *img, unsigned char *chunk, int from, int to)
{
    printf("Rebuild: from %d to %d\n", from, to);
    for (int i = from, j = 0; i <= to; i++, j++)
    {
        img[i] = chunk[j];
    }
}

int main(int argc, char *argv[])
{

    const char *pathImg = "img/originals/imageSmall.jpg";
    const char *outputImg = "img/results/imageSmall.jpg";

    // MPI Shit
    int numtasks, taskid, numworkers, source, worker;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    numworkers = numtasks - 1;

    // MASTER!! ... MASTER!!
    if (taskid == 0)
    {
        int width, height, channels, rows;
        int img_size;

        unsigned char *pre_img = stbi_load(pathImg, &width, &height, &channels, 1);
        if (pre_img == NULL)
        {
            printf("Error loading the image\n");
            exit(1);
        }
        // Convert the image to gray-scale, if the image contains a transparency value, it has 2 channels
        img_size = width * height * channels;
        img = (unsigned char *)malloc(img_size * sizeof(unsigned char));
        strncpy(img, pre_img, img_size);

        int initial, ending;

        rows = img_size / numworkers;
        initial = 0;
        ending = (initial + rows - 1);

        printf("sending!\n");

        for (worker = 1; worker <= numworkers; worker++)
        {
            MPI_Send(&width, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&initial, 1, MPI_INT, worker, 2, MPI_COMM_WORLD);
            MPI_Send(&ending, 1, MPI_INT, worker, 3, MPI_COMM_WORLD);
            MPI_Send(&img_size, 1, MPI_INT, worker, 4, MPI_COMM_WORLD);
            MPI_Send(img, img_size, MPI_UNSIGNED_CHAR, worker, 5, MPI_COMM_WORLD);
            initial = (worker - 1) * rows;
            ending = (initial + rows - 1);
            if (worker + 1 == numworkers)
            {
                ending = img_size - 1;
            }
        }
        printf("MASTER: info sent!\n");
        printf("MASTER: img size: %d\n", img_size);
        printf("MASTER: width %d height %d\n", width, height);
        printf("MASTER: receiving results!\n");
        int offset = 0;
        for (worker = 1; worker <= numworkers; worker++)
        {
            printf("MASTER: receiving from: %d\n", worker);

            MPI_Recv(&initial, 1, MPI_INT, worker, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&ending, 1, MPI_INT, worker, 1, MPI_COMM_WORLD, &status);
            printf("MASTER - %d: received offsets %d %d\n", worker, initial, ending);

            int npixels = (ending - initial + 1);
            unsigned char *img_chunk = (unsigned char *)malloc(npixels * sizeof(unsigned char));
            printf("%d: result size %d\n", taskid, rows);
            MPI_Recv(img_chunk, npixels, MPI_UNSIGNED_CHAR, worker, 2, MPI_COMM_WORLD, &status);
            printf("MASTER: received from: %d\n", worker);

            rebuild_img(img, img_chunk, initial, ending);

            printf("MASTER: freeing chunk!\n");
            free(img_chunk);
        }
        printf("received!\n");

        // Calculate time taken and output

        printf("writing img\n");
        stbi_write_jpg(outputImg, width, height, 1, img, 100);
        printf("freeing\n");
        stbi_image_free(pre_img);
        printf("MASTER: Freed pre_img!\n");
        free(img);
        printf("end!\n");
    }
    // good ol slave
    if (taskid > 0)
    {
        int width, height, channels, rows;
        int img_size;
        int initial, ending;
        int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
        source = 0;

        // Receive parameters
        MPI_Recv(&width, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&height, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&initial, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&ending, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&img_size, 1, MPI_INT, source, 4, MPI_COMM_WORLD, &status);

        unsigned char *this_img = (unsigned char *)malloc(img_size * sizeof(unsigned char));
        printf("%d: receiving img\n", taskid);
        MPI_Recv(this_img, img_size, MPI_UNSIGNED_CHAR, source, 5, MPI_COMM_WORLD, &status);
        printf("%d: received img\n", taskid);

        int npixels = (ending - initial + 1);
        printf("%d: img size: %d\n", taskid, img_size);
        printf("%d: number of pixels: %d\n", taskid, npixels);

        // Apply conv
        unsigned char *img_chunk = (unsigned char *)malloc(npixels * sizeof(unsigned char));

        int index = initial;

        for (int k = initial, i = 1; k < ending; k++, i++)
        {
            if (taskid == 2)
            {
                //printf("%d: k = %d, iteration %d in (%d - %d = %d)\n", taskid, k, i, initial, ending, npixels);
            }
            size_t y = index / height;
            size_t x = index % width;
            int acc = 0;
            for (int i = 0; i < 9; i++)
            {
                int mask_dim = 3;

                if (x == 0 && y == 0)
                {
                }
                else if (x == 0 && y == height - 1)
                {
                }
                else if (x == width - 1 && y == 0)
                {
                }
                else if (x == width - 1 && y == height - 1)
                {
                }
                else if (x == 0)
                {
                }
                else if (y == 0)
                {
                }
                else if (x == width - 1)
                {
                }
                else if (y == height - 1)
                {
                }
                else
                {
                    size_t dy = (i / mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));
                    size_t dx = (i % mask_dim) - (((mask_dim / 2) - 1) + (mask_dim % 2));

                    uint32_t value = ((uint32_t)(this_img[k + dx + (dy * width)])) * mask[i];

                    acc += value;
                }
            }
            // Delimits the value for the pixel to black or white for edge detection
            if (acc < 0)
            {
                acc = 0;
            }
            else if (acc > 255)
            {
                acc = 255;
            }
            img_chunk[k] = (unsigned char)acc;
            index++;
        }
        printf("%d: sending results\n", taskid);
        // Send results

        MPI_Send(&initial, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
        MPI_Send(&ending, 1, MPI_INT, source, 1, MPI_COMM_WORLD);
        printf("%d: sending img\n", taskid);
        MPI_Send(img_chunk, npixels, MPI_UNSIGNED_CHAR, source, 2, MPI_COMM_WORLD);
        printf("%d: results sent!\n", taskid);
        //free(this_img);
        printf("%d: freed img!\n", taskid);
        //free(img_chunk);
        printf("%d: finished all tasks!\n", taskid);
    }

    MPI_Finalize();

    return 0;
}