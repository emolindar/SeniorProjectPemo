#include <QImage>
#include <QColor>
#include "Image.h"
#include <algorithm>
#include <cmath>

Image QtoImage(const QImage& img){
    Image result(img.width(), img.height());

    for(int i=0; i<img.width();i++){
        for(int j=0; j<img.height(); j++){
            QColor colors = img.pixelColor(i,j);
            result.setPixel(i,j, Pixel(colors.redF(), colors.greenF(), colors.blueF()));
        }
    }

    return result; 
}


//move this to filters after you know it works 
void exposure(Image& img, float value){
    float factor = std::pow(2,value);

    for (int i=0; i<img.width(); i++){
        for (int j=0; j<img.height(); j++){
            img.setPixel(i,j,Pixel(img.getPixel(i,j).red() * factor, img.getPixel(i,j).green() * factor, img.getPixel(i,j).blue() * factor));
        }
    }
}

QImage ImagetoQ(const Image& img){
    QImage result(img.width(), img.height(), QImage::Format_RGBA8888);
    
    for(int i=0; i<img.width(); i++){
        for(int j=0; j<img.height(); j++){
            Pixel pixel = img.getPixel(i,j);
            QColor color = QColor::fromRgbF(
                std::clamp(pixel.red(), 0.0f, 1.0f),
                std::clamp(pixel.green(), 0.0f, 1.0f),
                std::clamp(pixel.blue(), 0.0f, 1.0f)
            );
            result.setPixelColor(i,j,color);    
        }
    }


    return result; 
}