#include "exposure.h"


void exposure(Image& img, float value){
    //this function calculates and applies the change to the colors of an image that correspond to a change in exposure
    float factor = std::pow(2,value);

    for (int i=0; i<img.width(); i++){
        for (int j=0; j<img.height(); j++){
            img.setPixel(i,j,Pixel(img.getPixel(i,j).red() * factor, img.getPixel(i,j).green() * factor, img.getPixel(i,j).blue() * factor));
        }
    }
}


