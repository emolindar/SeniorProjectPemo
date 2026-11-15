#include "dowscale.h"
#include <cmath>

//benchmarking
#include <iostream>
#include <chrono>

float avg_of_4_colors(float c1, float c2, float c3, float c4){
    /*this function calculates the average of 4 colors -- for example the average of 4 hues of red
        takes 4 floats,
        returns one float, which corresponds to the average of the 4
    */

    float result = (c1 + c2 + c3 + c4)/4.0f;
    return result;

}


Image downscale_image(const Image& img, int result_width, int result_height){
    /*this function will return a 4 times downscaled version of the provided image
     * takes an Image object
     * returns a downscaled image object
    */

    auto start = std::chrono::high_resolution_clock::now();
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

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Block Averaging took: " << elapsed.count() << "ms\n";
    return result;
}


//helper functino that returns the max of two numbers, so I don't import another library
float max_of_two(float a, float b){
    if (a>=b){
        return a;
    }
    return b;

}
//add a min of two helper function
float min_of_two(float a, float b){
    if (a<=b){
        return a;
    }
    return b;
}

//helper function for Lanczo's weight:
float sinc(float num){
    float pi = 2 * std::acos(0.0);
    if (num == 0){
        return 1;
    }
    return sin(pi * num) / (pi * num);
}

//helper function to calculate the weight based on the Lanczo kernel
float lanczoWeight(float distance, float area_radius){
    if (distance == 0){
        return 1.0f;
    }else if(distance >= area_radius){
        return 0.0f;
    }

    return sinc(distance) * sinc(distance/ area_radius);
}

//lanczos resampling
Image downscale_image_lanczos(Image& img, int result_width, int result_height){
    //this is another downscaling algorithm which should provide us with a more efficient result,
    //along with ability to scale, not hardcoded scale like the simpler downscaling algorithm I do have

    //exmplanation of my algorithm:
    /*
     * imagine that you take an empty downscaled image ( ie some pixels) that are exactly the desired width and hight
     * then grab the corners of that image and stretch is out (pixels remain as dots, only the space between them gets stretched)
     * so that the corners of the downscaled image match up with the corners of the og image
     * the pixels of the downscaled image would probably sit someone inbetween the pixels of the og image.
     * then imagine a square area around each downscaled pixel. That is the neighborhood we want to interpolate
     * and take the value of the corresponding place of the downscaled pixel
     */

    auto start = std::chrono::high_resolution_clock::now();
    // in the thesis -- compare the different downscaling algorithms perform


    Image result(result_width,result_height);

    float radius = 2 * img.width()/result_width;
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


            int interp_width_min = std::floor(max_of_two((float)(corr_width - std::sqrt(radius)/2.0f) , 0.0f));
            int interp_width_max = std::floor(min_of_two((float)(corr_width + std::sqrt(radius)/2.0f) , (float)(img.width()-1)));

            int interp_height_min = std::floor(max_of_two((float)(corr_height - std::sqrt(radius)/2.0f) , 0.0f));
            int interp_height_max = std::floor(min_of_two((float)(corr_height + std::sqrt(radius)/2.0f) , (float)(img.height()-1)));


            /* ^
             * |color value
             * |     /
             * |    /
             * |   /y(height)
             * |  /
             * | /                    x (width)
             * |/________________________>
             */


            float weighted_sum_red = 0;
            float weighted_sum_green = 0;
            float weighted_sum_blue = 0;
            float total_weight =0 ;
            for( int x = interp_width_min; x<=interp_width_max; x++){
                float lanczo_weight = 0;
                for( int y = interp_height_min; y <= interp_height_max; y++){

                    //interpolation given the set of points that I have and then approximate the value at the correlational position
                    // -- talk about different ways of approximation and go through the math of proving that the Lanzo's weighted average
                    // is going to be approximately close to the approximation using interpolation -- GO THROUGH THE MATH
                    // Nyquist-Shannon sampling theorem


                    //Using Lanczo's weighted values -- closer to 0 the further away from the point that we go

                    float distance_to_point = std::sqrt( std::pow((x-corr_width),2) + std::pow( y-corr_height,2));

                    lanczo_weight = lanczoWeight(distance_to_point, std::sqrt(radius)/2.0f);

                    weighted_sum_red += img.getPixel(x,y).red() * lanczo_weight;
                    weighted_sum_green += img.getPixel(x,y).green() * lanczo_weight;
                    weighted_sum_blue += img.getPixel(x,y).blue() * lanczo_weight;

                    total_weight += lanczo_weight;
                }



            }


            result.setPixel(i,j,Pixel(
                                      std::clamp(weighted_sum_red / total_weight, 0.0f, 1.0f),
                                        std::clamp(weighted_sum_green / total_weight, 0.0f, 1.0f),
                                        std::clamp(weighted_sum_blue / total_weight, 0.0f, 1.0f)));


        }
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Lanczos downscaling to ( " << result_width << ", " << result_height << ") took: " << elapsed.count() << " ms\n";
    return result;
}



Image twoPassLanczos(const Image& img, int result_width, int result_height){
    //returns a downscaled image
    //that has been resampled using Lanczos resampling, but done in two passes


    /*doing only a part of what the downscale_image_lanczos does
    *this is another iteration of the lanczos resampling,
    *but doing it with one horizontal and one vertical pass
    *
    *-- this is the horizontal pass
    */
    auto start = std::chrono::high_resolution_clock::now();
    Image horizontal_result(result_width, img.height());

    //dynamically assigned radius

    if(result_width == 0) result_width = 1;
    float radius = std::max(2 * (float)img.width()/result_width, 2.0f);
    float scale = radius/2;
    for(int i=0; i<horizontal_result.width(); i++){
        for(int j=0; j< horizontal_result.height(); j++){

            //correlational width
            float corr_height = ( (float)img.height()/(float)horizontal_result.height() ) * j;
            float corr_width = ( (float)img.width()/(float)horizontal_result.width() ) * i;


            //boundaries  NOT CORRECT FIX
            int interp_width_min = std::floor(max((corr_width - radius) , 0.0f));
            int interp_width_max = std::floor(min((corr_width + radius) , (float)(img.width()-1)));

            float weighted_sum_red = 0;
            float weighted_sum_green = 0;
            float weighted_sum_blue = 0;
            float total_weight =0 ;



            for(int x = interp_width_min; x<= interp_width_max; x++){
                //performing the resampling
                /*instead of taking a pixel based on a location,
                 * i know the width, so I can take the image_data 1d vector,
                 * start from the pixel that is <radius> left of the correlational position (corr_width, img.height()),
                 * and loop through radius*2 pixels;
                 * no need for nested loop
                 */

                float distance_to_point = std::abs(corr_width - x);
                float lanczos_weight = lanczoWeight(distance_to_point/scale, 2.0f);

                const Pixel& p = img.getPixel(x,j);
                weighted_sum_red += p.red() * lanczos_weight;
                weighted_sum_green += p.green() * lanczos_weight;
                weighted_sum_blue += p.blue() * lanczos_weight;
                total_weight+= lanczos_weight;
            }
            if (std::abs(total_weight) < 1e-6f) total_weight = 1.0f;
            horizontal_result.setPixel(i,j,Pixel(
                                    std::clamp(weighted_sum_red/ total_weight,0.0f,1.0f),
                                    std::clamp(weighted_sum_green/ total_weight,0.0f,1.0f),
                                    std::clamp(weighted_sum_blue/ total_weight,0.0f,1.0f)));



        }
    }

    /*
     * this is the other part of the algorithm
     *
     * -- vertical pass
     */

    Image result(result_width, result_height);

    //the vertical pass  might still have a different radius,
    //based on the aspect ratio of the image

    if (result_height == 0 ) result_height = 1;
    radius = std::max(2.0f * (float)((float)horizontal_result.height()/result_height), 2.0f);
    scale = radius/2;
    for(int i=0; i<result.width(); i++){
        for(int j=0; j< result.height(); j++){

            //correlational pos
            float corr_height = ( (float)img.height()/(float)result.height() ) * j;
            float corr_width = ( (float)img.width()/(float)result.width() ) * i;

            //boundaries
            int interp_height_min = std::floor(std::max((corr_height - radius) , 0.0f));
            int interp_height_max = std::floor(std::min((corr_height + radius) , (float)img.height()-1));

            float weighted_sum_red = 0;
            float weighted_sum_green = 0;
            float weighted_sum_blue = 0;
            float total_weight =0 ;



            for(int y = interp_height_min; y<= interp_height_max; y++){
                //performing the resampling
                /* trhe vertical pass is a bit more tricky
                 * but it would be the corr_pos - radius* width
                 * and corr_pos + radius * width;
                 *
                 */


                //have to divide because
                /* imagine image data:
                 * [ * * 0 *
                 *   * * * *
                 *   * * 0 *
                 *   * * * *]
                 *
                 *   so the logic is that the distance between those should be 1 ( there is one pixel b/w them)
                 *   but then if i do corr_pos(The second 0) -x (the first one ) thats 7
                 *    ----=> then we divide that by the img.width() to get the actual distance
                 */
                float distance_to_point = std::abs(y - corr_height);
                float lanczos_weight = lanczoWeight(distance_to_point / scale, 2.0f);


                const Pixel& p = horizontal_result.getPixel(i,y);
                weighted_sum_red += p.red() * lanczos_weight;
                weighted_sum_green += p.green() * lanczos_weight;
                weighted_sum_blue += p.blue() * lanczos_weight;
                total_weight+= lanczos_weight;
            }
            if (std::abs(total_weight) < 1e-6f) total_weight = 1.0f;
            result.setPixel(i,j,Pixel(
                                                 std::clamp(weighted_sum_red/ total_weight,0.0f,1.0f),
                                                 std::clamp(weighted_sum_green/ total_weight,0.0f,1.0f),
                                                 std::clamp(weighted_sum_blue/ total_weight,0.0f,1.0f)));



        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Two Pass Lanczos Downscaling to ( " << result_width << ", " << result_height << ") took: " << elapsed.count() << " ms\n";
    return result;
}
