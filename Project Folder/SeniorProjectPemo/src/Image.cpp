#include "Image.h"
#include <vector>

void Pixel::setPixel(float r, float g, float b){
        rgb_.clear();
        this->rgb_ = {
                       std::clamp(r, 0.0f, 1.0f),
                       std::clamp(g, 0.0f, 1.0f),
                       std::clamp(b, 0.0f, 1.0f)
        };
    }


//constructors of Image
Image::Image(int width,int height) : width_(width), height_(height) {
    image_data_.resize(width, vector<Pixel>(height, Pixel(0,0,0)));
}
