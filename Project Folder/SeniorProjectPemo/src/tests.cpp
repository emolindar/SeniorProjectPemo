#include <cassert>
#include <iostream>



#include "Image.h"
#include "versionGraph.h"
#include "algorithms/contrast.h"
#include "algorithms/exposure.h"
#include "algorithms/highlights.h"
#include "algorithms/shadows.h"
#include "algorithms/dowscale.h"


bool approximatelyEqual(float a, float b){
    // adding a tolerance to see if two values are approx. equal
    //with some room for inperceptable error
    return std::fabs(a-b) <= 1e-4f;
}

bool imagesApproximatelyEqual(Image& a, Image& b){
    //checking if two images are approx. equal
    //this function goes through every pixel and makes sure that they are approx. the same
    if(a.width() != b.width() or a.height() != b.height()){
        return false;
    }

    for(int i=0; i<a.width(); i++){
        for(int j=0; j<a.height();j++){
            const Pixel& pa = a.getPixel(i,j);
            const Pixel& pb = b.getPixel(i,j);
            if(!approximatelyEqual(pa.red(), pb.red() ||
                !approximatelyEqual(pa.blue(), pb.blue() ||
                !approximatelyEqual(pa.green(), pb.green()) ))){

                return false;
            }
        }
    }
    return true;
}


void assertZeroAdjustment(){

    //creating an example image
    Image first(100,100);
    for(int i=0; i<first.width(); i++){
        for(int j=0; j<first.height();j++){
            first.setPixel(i,j,Pixel(std::rand()%2, std::rand()%2, std::rand()%2));
        }
    }

    Image afterExposure = first;
    exposure(afterExposure, 0.0f);
    assert(imagesApproximatelyEqual(first, afterExposure));

    Image afterContrast = first;
    contrast(afterContrast, 0.0f);
    assert(imagesApproximatelyEqual(first, afterContrast));

    Image afterHighlights = first;
    highlights(afterHighlights, 0.0f);
    assert(imagesApproximatelyEqual(first, afterHighlights));

    Image afterShadows = first;
    shadows(afterShadows, 0.0f);
    assert(imagesApproximatelyEqual(first, afterShadows));

    std::cout<<"\n-----------Zero Adjustment Tests Complete-----------\n";

}


void assertAfterDownscaling(){

    //making it a constant color so making sure that it remains the same colour
    Image first(100,100);
    for(int i=0; i<first.width(); i++){
        for(int j=0; j<first.height();j++){
            first.setPixel(i,j,Pixel(0.5f, 0.5f, 0.5f));
        }
    }

    //block averaging
    Image downscaledBlockAverage = downscale_image(first,50,50);
    assert(downscaledBlockAverage.width()==50 && downscaledBlockAverage.height()==50);

    for(int i=0; i<downscaledBlockAverage.width(); i++){
        for(int j=0; j<downscaledBlockAverage.height(); j++){
            const Pixel& p = downscaledBlockAverage.getPixel(i,j);

            assert(approximatelyEqual(p.red(), 0.5f));
            assert(approximatelyEqual(p.green(), 0.5f));
            assert(approximatelyEqual(p.blue(), 0.5f));
        }
    }


    //lanczos
    Image downscaledLanczos = downscale_image_lanczos(first,50,50);
    assert(downscaledLanczos.width()==50 && downscaledLanczos.height()==50);

    for(int i=0; i<downscaledLanczos.width(); i++){
        for(int j=0; j<downscaledLanczos.height(); j++){
            const Pixel& p = downscaledLanczos.getPixel(i,j);

            assert(approximatelyEqual(p.red(), 0.5f));
            assert(approximatelyEqual(p.green(), 0.5f));
            assert(approximatelyEqual(p.blue(), 0.5f));
        }
    }

    //two pass lanczos
    Image downscaledTwoPassLanczos = twoPassLanczos(first,50,50);
    assert(downscaledTwoPassLanczos.width()==50 && downscaledTwoPassLanczos.height()==50);

    for(int i=0; i<downscaledTwoPassLanczos.width(); i++){
        for(int j=0; j<downscaledTwoPassLanczos.height(); j++){
            const Pixel& p = downscaledTwoPassLanczos.getPixel(i,j);

            assert(approximatelyEqual(p.red(), 0.5f));
            assert(approximatelyEqual(p.green(), 0.5f));
            assert(approximatelyEqual(p.blue(), 0.5f));
        }
    }

    std::cout<<"\n-----------Downscaling Tests Complete-----------\n";

}


int main(){
    assertZeroAdjustment();
    assertAfterDownscaling();
    std::cout<<"Tests done";
    return 0;
}