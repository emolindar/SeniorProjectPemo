#include "contrast.h"
#include <cmath>

void contrast(Image& img, float value){

    float scale = std::tan((value + 1.0) * 3.145298/4.0);

    for( int i =0; i< img.width(); i++){
        for (int j=0; j< img.height(); j++){
            img.setPixel(
                i,
                j,
                Pixel(
                (img.getPixel(i,j).red() - 0.5) * scale + 0.5,
                (img.getPixel(i,j).green() - 0.5) * scale + 0.5,
                (img.getPixel(i,j).blue() - 0.5) * scale + 0.5));
        }
    }
}