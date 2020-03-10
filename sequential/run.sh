#!/bin/bash

IMAGE_SOURCE="img/originals"
IMAGE_RESULTS="img/results/sequential"

smallImageTime=$(./convImage "$IMAGE_SOURCE/imageSmall.jpg" "$IMAGE_RESULTS/resultSmall.jpg")
echo -ne "Convolution for smallImage using the sequential algorithm took $smallImageTime seconds\n"

normalImageTime=$(./convImage "$IMAGE_SOURCE/imageNormal.jpg" "$IMAGE_RESULTS/resultNormal.jpg")
echo -ne "Convolution for normalImage using the sequential algorithm took $normalImageTime seconds\n"

largeImageTime=$(./convImage "$IMAGE_SOURCE/imageBig.jpg" "$IMAGE_RESULTS/resultBig.jpg")
echo -ne "Convolution for largeImage using the sequential algorithm took $largeImageTime seconds\n"
