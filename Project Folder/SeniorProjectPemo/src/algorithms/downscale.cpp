#include "dowscale.h"

float avg_of_4_colors(float c1, float c2, float c3, float c4){
    /*this function calculates the average of 4 colors -- for example the average of 4 hues of red
        takes 4 floats,
        returns one float, which corresponds to the average of the 4
    */

    float result = (c1 + c2 + c3 + c4)/4.0f;
    return result;

}


Image downscale_image(const Image& img){
    /*this function will return a 4 times downscaled version of the provided image
     * takes an Image object
     * returns a downscaled image object
    */


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

        //accounting for size not divisible by 4
        if (i >= img.width()){
            break;
        }

        for (int j = 0; j< img.height()-1; j+=2){

            //accounting for size not divisible by 4
            if (j >= img.height()){
                break;
            }

            //calculating the average of the 4 pixels

            Pixel result_pixel(
                avg_of_4_colors(img.getPixel(i,j).red(), img.getPixel(i+1, j).red(), img.getPixel(i,j+1).red(), img.getPixel(i+1, j+1).red()),
                avg_of_4_colors(img.getPixel(i,j).green(), img.getPixel(i+1, j).green(), img.getPixel(i,j+1).green(), img.getPixel(i+1, j+1).green()),
                avg_of_4_colors(img.getPixel(i,j).blue(), img.getPixel(i+1, j).blue(), img.getPixel(i,j+1).blue(), img.getPixel(i+1, j+1).blue())
                );

            result.setPixel(i,j, result_pixel);
        }
    }

    return result;
}