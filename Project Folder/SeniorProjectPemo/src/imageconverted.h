#include <QImage>
#include <QColor>
#include "Image.h"

Image QtoImage(QImage img){
    Image result = new Image(img.width(), img.height());

    for(int i=0; i<img.width();i++){
        for(int j=0; j<img.height(); j++){
            QColor colors = img.pixelColor();
            result.setPixel(colors.RedF(), colors.GreenF(), colors.BlueF());
        }
    }

    return result; 
}

QImage ImagetoQ(Image img){

}