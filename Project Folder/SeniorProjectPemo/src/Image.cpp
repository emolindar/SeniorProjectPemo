#include "Image.h"
#include <vector>

void Pixel::setPixel(float r, float g, float b){
        rgb_.clear();
        this->rgb_ = {r,g,b};
    }


//constructors of Image
Image::Image(int width,int height) : width_(width), height_(height) {
    image_data_.resize(width, vector<Pixel>(height, Pixel(0,0,0)));
}
