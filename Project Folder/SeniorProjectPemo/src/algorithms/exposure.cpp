#include "exposure.h"


void exposure(Image& img, float value){
    //this function calculates and applies the change to the colors of an image that correspond to a change in exposure


    //the algorithm is quite simple -- simply shift the color values of each pixel by a specified factor
    float factor = std::pow(2,value);

    for (int i=0; i<img.width(); i++){
        for (int j=0; j<img.height(); j++){
            img.setPixel(i,j,Pixel(
                                   std::clamp(img.getPixel(i,j).red() * factor, 0.0f, 1.0f),
                                   std::clamp(img.getPixel(i,j).green() * factor, 0.0f, 1.0f),
                                   std::clamp(img.getPixel(i,j).blue() * factor, 0.0f, 1.0f)
                                     ));
        }
    }
}


