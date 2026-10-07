#ifndef DOWSCALE_H
#define DOWSCALE_H

#include "../Image.h"

Image downscale_image(const Image& img);

float avg_of_4_colors(float c1, float c2, float c3, float c4);

//adding bicubic interpolation

Image downscale_image_bicubic(const Image& img);

#endif // DOWSCALE_H
