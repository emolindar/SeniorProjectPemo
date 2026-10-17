#include "shadows.h"
#include "extraFuncts.h"


void shadows(Image& img, float value){

    for (int i =0; i< img.width(); i++){
        for (int j = 0; j< img.height(); j++){
            Pixel p = img.getPixel(i,j);
            float lum = luminance(
                p.red(),
                p.green(),
                p.blue());
            float weight = (1 - smoothStep(0.0, 0.5, lum))*value;



            float red, green, blue;
            if(weight>0){
                red = p.red() +weight*(1-p.red());
                blue = p.blue() +weight*(1-p.blue());
                green = p.green() + weight*(1-p.green());
            }else{
                red = p.red() + weight*p.red();
                blue = p.blue() + weight*p.blue();
                green = p.green() + weight*p.green();
            }

            img.setPixel(i,j,Pixel(red, green, blue));

        }
    }
}