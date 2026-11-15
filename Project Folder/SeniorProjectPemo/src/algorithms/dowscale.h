#ifndef DOWSCALE_H
#define DOWSCALE_H

#include "../Image.h"

Image downscale_image(const Image& img, int result_width, int result_height);
//^ this function has unused parameters such that I can use it in the downscale function in the mainwindow.h class
float avg_of_4_colors(float c1, float c2, float c3, float c4);

//adding bicubic interpolation

Image downscale_image_lanczos(Image& img, int result_width, int result_height);


Image twoPassLanczos(const Image& img, int result_width, int result_height);
#endif // DOWSCALE_H
