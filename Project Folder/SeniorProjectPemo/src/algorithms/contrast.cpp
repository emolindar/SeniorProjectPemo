#include "contrast.h"
#include <cmath>

void contrast(Image& img, float value){

    float scale = std::tan((value + 1.0) * M_PI/4.0);

    for( int i =0; i< img.width(); i++){
        for (int j=0; j< img.height(); j++){

            const Pixel& p = img.getPixel(i,j);

            img.setPixel(
                i,
                j,
                Pixel(
                (p.red() - 0.5) * scale + 0.5,
                (p.green() - 0.5) * scale + 0.5,
                (p.blue() - 0.5) * scale + 0.5));
        }
    }
}