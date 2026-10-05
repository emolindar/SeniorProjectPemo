#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "src/Image.h"
#include "src/imageconverter.h"
#include "src/algorithms/exposure.h"
#include <QSlider>


/*TODO
 * -> move all the function implementations to .cpp file, don't leave them in .h -- kinda done but keep doing
3. start working on the image version graph
4. add a refresh function, which refreshes the image every time a change is made ( i.e. the slider is moved)
5. add a way to export the image at the end
6. start thinking about optimizing all the functions, most just have a O(n^2)
    - for the exposure -- downscale the quality of the displayed photo and only update it once the slider is released
                -- downscaling algorithm --- this will be very important - bicubic downscaling(possible)  or high quality Catmull-Rom
                        - could also go with something more original -- scale 4 times, so calculate the average color of 4 pixels and replace them with that



            -- update on point 6 --- implemented the downscaling, need to make it more dynamic ( based on the original size of the image) and then I
                need to make it so that while holding the slider, only the downscaled version is shown, and the changes are reflected on it, and then
                when you release the slider the changes are reflected onto the full image
*/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QString path = "/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg";
    QImage loaded(path);
    if(loaded.isNull()){
        std::cout << "Error" << std::endl;
        return; 
    }

    images.push_back(QtoImage(loaded));//og image (images[0]
    images.push_back(images[0]); //edited image images[1]


    previews.push_back(downscale_image(images[0]));
    previews.push_back(downscale_image(images[1]));

    //testing exposure
    exposureSlider->setRange(-300, 300);
    exposureSlider->setValue(0);

    connect(exposureSlider, &QSlider::valueChanged, this, &MainWindow::onExposureChange);


    applyExposureSlider(exposureSlider->value(), images[1], images[0]);


//    QImage img_output = ImagetoQ(images[1]);

 //   image_viewer->setPixmap(QPixmap::fromImage(img_output));
    
    //testing the downscaled preview
    QImage preview_output = ImagetoQ(previews[1]);
    preview_output = preview_output.scaled(
                      800,
                      600,
                      Qt::KeepAspectRatio,
                    Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(preview_output));



    //QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    //image_viewer->setPixmap(pix);
    splitter->addWidget(image_viewer);
    splitter->addWidget(listview);
    splitter->addWidget(exposureSlider);
    setCentralWidget(splitter);
//ui->setupUi(this);
}



void MainWindow::applyExposureSlider(int slider_value, Image& etd, Image& og){
    float ex_value = slider_value/300.0f; // this value determines the amount of exposure change-- fine tune
    etd = og;
    exposure(etd, ex_value);

}

void MainWindow::onExposureChange(int value){
    applyExposureSlider(value, images[1], images[0]);
    refreshDisplay();
}


void MainWindow::refreshDisplay(){
    QImage output = ImagetoQ(images[1]);
    //adding a scaling so that the image is always the same size, no matter the resolution it has. Will make this dynamic in the future
    output = output.scaled(
                       800,
                       600,
                       Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}


MainWindow::~MainWindow()
{
    delete ui;
}
