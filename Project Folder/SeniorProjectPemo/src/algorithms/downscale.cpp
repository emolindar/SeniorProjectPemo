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
    Image result(img.width()/2, img.height()/2);




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

            result.setPixel(i/2,j/2, result_pixel);
        }
    }

    return result;
}


//helper functino that returns the max of two numbers, so I don't import another library
float max_of_two(float a, float b){
    if (a>=b){
        return a;
    }
    return b;

}

//bicubic interpolation
Image downscale_image_bicubic(const Image& img, int result_width, int result_height, int interpolation_size){
    //this is another downscaling algorithm which should provide us with a more efficient result,
    //along with ability to scale, not hardcoded scale like the simpler downscaling algorithm I do have


    Image result(result_width,result_height);

    //idea is that I will look at the pixel location of the result, then based on it's scale, ie width and height, find it's corresponding location in the og image
    //then,I will look at [interpolation_size] pixels around it and curve their rgb values, finding the values that would correspond to the relative position of
    //the resulting pixel.


    for(int i=0; i<result.width(); i++){
        for(int j=0; j< result.height(); j++){


            //calc corresponding loc of resulting pixel in og image

            float corr_width = ( (float)img.width()/(float)result.width() ) * i;
            float corr_height = ( (float)img.height()/(float)result.height() ) * j;


            //take those and find the surrounding pixels
            /*
             * iwmin    iwmax    ihmin
             * x x x x
             * x x x x
             * x x x x
             * x x x x          ihmax
            */

            int interp_width_min = std::floor(max_of_two((float)(corr_width - std::sqrt(interpolation_size)/2.0f) , 0.0f));
            int interp_width_max = std::floor(max_of_two((float)(corr_width + std::sqrt(interpolation_size)/2.0f) , (float)(img.width()-1)));

            int interp_height_min = std::floor(max_of_two((float)(corr_height - std::sqrt(interpolation_size)/2.0f) , 0.0f));
            int interp_height_max = std::floor(max_of_two((float)(corr_height + std::sqrt(interpolation_size)/2.0f) , (float)(img.height()-1)));


            /* ^
             * |color value
             * |     /
             * |    /
             * |   /j(height)
             * |  /
             * | /                    i (width)
             * |/________________________>
             */


        }
    }

    return result;
}
