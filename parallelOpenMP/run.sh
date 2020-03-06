#!/bin/bash

IMAGE_SOURCE="img/originals"
IMAGE_RESULTS="img/results/OpenMP"
THREAD_COUNT=16

read -p "Ammount of threads to use: " -r THREAD_COUNT 

smallImageTime=$(./convImage "$IMAGE_SOURCE/imageSmall.jpg" "$IMAGE_RESULTS/resultSmall.jpg" $THREAD_COUNT)
echo -ne "Convolution for smallImage using the OpenMP algorithm took $smallImageTime seconds using $THREAD_COUNT threads\n"

normalImageTime=$(./convImage "$IMAGE_SOURCE/imageNormal.jpg" "$IMAGE_RESULTS/resultNormal.jpg" $THREAD_COUNT)
echo -ne "Convolution for normalImage using the OpenMP algorithm took $normalImageTime seconds using $THREAD_COUNT threads\n"

largeImageTime=$(./convImage "$IMAGE_SOURCE/imageBig.jpg" "$IMAGE_RESULTS/resultBig.jpg" $THREAD_COUNT)
echo -ne "Convolution for largeImage using the OpenMP algorithm took $largeImageTime seconds using $THREAD_COUNT threads\n"
