#!/bin/bash

IMAGE_SOURCE="../img/originals"
IMAGE_RESULTS="../img/results/OpenMP"

# smallImageTime=$(./convImage "$IMAGE_SOURCE/imageSmall.jpg" "$IMAGE_RESULTS/resultSmall.jpg")
(./convImage "$IMAGE_SOURCE/imageSmall.jpg" "$IMAGE_RESULTS/resultSmall.jpg" 8)
# echo -ne "Convolution for smallImage using the OpenMP algorithm took $smallImageTime seconds\n"

# normalImageTime=$(./convImage "$IMAGE_SOURCE/imageNormal.jpg" "$IMAGE_RESULTS/resultNormal.jpg")
(./convImage "$IMAGE_SOURCE/imageNormal.jpg" "$IMAGE_RESULTS/resultNormal.jpg" 8)
# echo -ne "Convolution for normalImage using the OpenMP algorithm took $normalImageTime seconds\n"

# largeImageTime=$(./convImage "$IMAGE_SOURCE/imageBig.jpg" "$IMAGE_RESULTS/resultBig.jpg")
(./convImage "$IMAGE_SOURCE/imageBig.jpg" "$IMAGE_RESULTS/resultBig.jpg" 8)
# echo -ne "Convolution for largeImage using the OpenMP algorithm took $largeImageTime seconds\n"
