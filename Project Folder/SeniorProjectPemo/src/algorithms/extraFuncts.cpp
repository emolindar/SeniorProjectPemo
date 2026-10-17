#include "extraFuncts.h"
#include <algorithm>

float luminance(float red, float green, float blue){
    //this function returns the luminance of a pixel
    //cited in the thesis
    return 0.299*red + 0.587*green + 0.114*blue;
}

float smoothStep(float edge0, float edge1, float x){
    //Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and
    //performs smooth Hermite interpolation between 0 and 1
    //when edge0 < x < edge1. This is useful in cases where
    //you would want a threshold function with a smooth transition.

    //this is cited in the thesis

    float t = std::clamp( (x - edge0)/ (edge1-edge0), 0.0f, 1.0f );
    return t*t*(3-2*t);
}
