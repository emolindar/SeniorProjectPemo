#include "dowscale.h"

Image downscale_image(const Image& img){
    //trying the original(i don't know how original it is but i am trying this by myself) idea
    //of downscaling using the logic that it will scale the image down 4 times
    //so that it will replace every block of 4 pixels with their avg color


    // in order for this to work the dimensions need to be divisible by 4 (or I at least need to account for other cases)
    Image result(img.width()/4, img.height()/4);




    /* - visual representation for myself to think
     * - x is where the loop will look and the 0 are the pixels it misses
        x 0 x 0 x 0 x 0
        0 0 0 0 0 0 0 0
        x 0 x 0 x 0 x 0
        0 0 0 0 0 0 0 0

    */

    for(int i=0; i<img.width()-1; i+=2){
        for (int j = 0; j< img.height()-1; j+=2){

        }
    }

    return result;
}