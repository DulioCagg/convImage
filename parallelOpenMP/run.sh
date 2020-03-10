#!/bin/bash

IMAGE_SOURCE="../img/originals"
IMAGE_RESULTS="../img/results/OpenMP"

(./convImage "$IMAGE_SOURCE/imageSmall.jpg" "$IMAGE_RESULTS/resultSmall.jpg")

(./convImage "$IMAGE_SOURCE/imageNormal.jpg" "$IMAGE_RESULTS/resultNormal.jpg")

(./convImage "$IMAGE_SOURCE/imageBig.jpg" "$IMAGE_RESULTS/resultBig.jpg")
