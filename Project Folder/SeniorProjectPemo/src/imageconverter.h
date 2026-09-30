#include <QImage>
#include <QColor>
#include "Image.h"
#include <algorithm>

Image QtoImage(const QImage& img){
    Image result(img.width(), img.height());

    for(int i=0; i<img.width();i++){
        for(int j=0; j<img.height(); j++){
            QColor colors = img.pixelColor(i,j);
            result.image_data()[i][j].setPixel(colors.redF(), colors.greenF(), colors.blueF());
        }
    }

    return result; 
}

QImage ImagetoQ(const Image& img){
    QImage result(img.width(), img.height(), QImage::Format_RGBA8888);
    
    for(int i=0; i<img.width(); i++){
        for(int j=0; j<img.height(); j++){
            Pixel pixel = img.getPixel(i,j);
            QColor color = QColor::fromRgbF(
                std::clamp(pixel.red(), 0, 1),
                std::clamp(pixel.green(), 0, 1),
                std::clamp(pixel.blue(), 0, 1)
            );
            result.setPixelColor(i,j,color);    
        }
    }

    return result; 
}