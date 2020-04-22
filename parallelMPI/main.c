#include "convImage.h"
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MPI_Status status;
unsigned char *img;
unsigned char *result_img;

// Assignes the values from worker chunk to the resulting image
void rebuild_img(unsigned char *img, unsigned char *chunk, int from, int to)
{
    for (int i = from, j = 0; i <= to; i++, j++)
    {
        img[i] = chunk[j];
    }
}

int main(int argc, char *argv[])
{

    const char *img_name = "imageBig";
    const char *pathImg = "img/originals/imageTestTest.jpg";
    const char *outputImg = "img/results/imageBig.jpg";

    // MPI parameters and intialization
    int numtasks, taskid, numworkers, source, worker;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    numworkers = numtasks - 1;

    // Code for Master task
    if (taskid == 0)
    {
        // Image parameters
        int width, height, channels, pixels_per_worker;
        int img_size;

        // Loads the image in grayscale
        img = stbi_load(pathImg, &width, &height, &channels, 1);
        if (img == NULL)
        {
            printf("Error loading the image\n");
            exit(1);
        }
        // Allocates the memory for the resulting image
        result_img = (unsigned char *)malloc((width * height) * sizeof(unsigned char));

        // The range and the number of pixels each worker is going to manipulate
        int initial, ending;
        pixels_per_worker = (int) (width * height) / numworkers;

        // Starts the count for time
        double start = MPI_Wtime();

        // For each worker, sends all the necessary paramters
        for (worker = 1; worker <= numworkers; worker++)
        {
            initial = (worker - 1) * pixels_per_worker;
            ending = (initial + pixels_per_worker) - 1;
            if (worker == numworkers)
            {
                ending = (width * height) - 1;
            }
            MPI_Send(&width, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, worker, 1, MPI_COMM_WORLD);
            MPI_Send(&initial, 1, MPI_INT, worker, 2, MPI_COMM_WORLD);
            MPI_Send(&ending, 1, MPI_INT, worker, 3, MPI_COMM_WORLD);
            MPI_Send(img, (width * height), MPI_UNSIGNED_CHAR, worker, 4, MPI_COMM_WORLD);
        }
        // For each worker, receive the chunk with the results and assign it to the result image
        for (worker = 1; worker <= numworkers; worker++)
        {
            MPI_Recv(&initial, 1, MPI_INT, worker, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&ending, 1, MPI_INT, worker, 1, MPI_COMM_WORLD, &status);

            // Calculates the size of the chink and allocates that memory
            int npixels = (ending - initial + 1);
            unsigned char *img_chunk = (unsigned char *)malloc(npixels * sizeof(unsigned char));

            MPI_Recv(img_chunk, npixels, MPI_UNSIGNED_CHAR, worker, 2, MPI_COMM_WORLD, &status);

            // Assignes the resulting values to the result image
            rebuild_img(result_img, img_chunk, initial, ending);

            free(img_chunk);
        }
        // Calculates the time it took to apply the convolution and prints it
        double end = MPI_Wtime();
        printf("%s, %d, %lf\n", img_name, numtasks, end - start);

        stbi_write_jpg(outputImg, width, height, 1, result_img, 100);
        stbi_image_free(img);
        free(result_img);
    }
    // Code for worker processes
    if (taskid > 0)
    {
        // Initializes the parameters to manipluate the image
        int width, height;
        int img_size;
        int initial, ending;
        int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
        source = 0;

        // Receive parameters
        MPI_Recv(&width, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&height, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&initial, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        MPI_Recv(&ending, 1, MPI_INT, source, 3, MPI_COMM_WORLD, &status);

        unsigned char *this_img = (unsigned char *)malloc((width * height) * sizeof(unsigned char));
        MPI_Recv(this_img, (width * height), MPI_UNSIGNED_CHAR, source, 4, MPI_COMM_WORLD, &status);

        // Calculates the size of the chink and allocates that memory
        int npixels = (ending - initial + 1);
        unsigned char *img_chunk = (unsigned char *)malloc(npixels * sizeof(unsigned char));

        // Apply conv
        int index = 0;
        for (int k = initial, i = 1; k < ending; k++, i++)
        {
            if (taskid == 2)
            {
            }
            size_t y = k / height;
            size_t x = k % width;
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
            img_chunk[index] = (unsigned char)acc;
            index++;
        }

        // Send results
        MPI_Send(&initial, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
        MPI_Send(&ending, 1, MPI_INT, source, 1, MPI_COMM_WORLD);
        MPI_Send(img_chunk, npixels, MPI_UNSIGNED_CHAR, source, 2, MPI_COMM_WORLD);
        free(this_img);
        free(img_chunk);
    }

    MPI_Finalize();

    return 0;
}
