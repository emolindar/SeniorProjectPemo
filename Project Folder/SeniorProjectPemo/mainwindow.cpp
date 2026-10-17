#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "src/Image.h"
#include "src/imageconverter.h"
#include "src/algorithms/exposure.h"
#include "src/algorithms/contrast.h"
#include "src/algorithms/highlights.h"
#include <QSlider>


/*TODO
 * -> move all the function implementations to .cpp file, don't leave them in .h -- kinda done but keep doing
-> start working on the image version graph - need to answer a few questions first

            -- when do I want the version to branch? Every change seems to be too much
            -- how will this be visualized
            -- in merging, which changes take priority? what if I take an average of the changes? Can I add weights to changes, so that deeperbranches
                    take some kind of priority and other changes are added on top?
            -- how will I record the type of changes made? Simply by the image or will I record the function call


-> add a way to export the image at the end


->  start thinking about optimizing all the functions, most just have a O(n^2)
    - for the exposure -- downscale the quality of the displayed photo and only update it once the slider is released
                -- downscaling algorithm --- this will be very important - bicubic downscaling(possible)  or high quality Catmull-Rom
                        - could also go with something more original -- scale 4 times, so calculate the average color of 4 pixels and replace them with that



            -- update on point 6 --- implemented the downscaling, need to make it more dynamic ( based on the original size of the image) and then I
                need to make it so that while holding the slider, only the downscaled version is shown, and the changes are reflected on it, and then
                when you release the slider the changes are reflected onto the full image


         -- generalize this to work for all features, not just exposure, so I don't have to write 4 functions that refresh the display every time I add a feature
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


    //testing exposure
    exposureSlider->setRange(-300, 300);
    exposureSlider->setValue(0);

    //this makes it so that the function onExposureChange is called when the value of the slider is changed
    connect(exposureSlider, &QSlider::valueChanged, this, &MainWindow::onExposureChangePreview);

    connect(exposureSlider, &QSlider::sliderReleased, this, [this]() {onExposureChange(exposureSlider->value());});
    //^ need to add lambda function which can grab the exposure slider value and then call the on exposure change function

    //attempting to add the contrast slider with the generalized function
    contrastSlider->setRange(-300, 300);
    contrastSlider->setValue(0);

    //connecting the contrastSlider with the modified function but with the generalized method for applying modifications
    connect(contrastSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(contrast, value); });
    //doing the same for the contrast preview
    connect(contrastSlider, &QSlider::sliderReleased, this, [this]() {onModificationChange(contrast,contrastSlider->value());});


    //highlights
    hightlightsSlider->setRange(-300, 300);
    hightlightsSlider->setValue(0);

    connect(hightlightsSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(highlights, value);});
    connect(hightlightsSlider, &QSlider::sliderReleased, this, [this](){onModificationChange(highlights, hightlightsSlider->value());} );


    //downscaling algorithm buttons choice
    averageDowncaling = new QRadioButton("Averaging pixel Values in a block of 4");
    bicubicDownscaling = new QRadioButton("Bicubic downscaling");
    averageDowncaling -> setChecked(true);
        //setting the default downscaling algorithm as the simple averaging algorithm
    downscalingFunction = downscale_image;
    downscalingButtonGroup = new QButtonGroup(this);
    downscalingButtonGroup->addButton(averageDowncaling);
    downscalingButtonGroup->addButton(bicubicDownscaling);

    connect(downscalingButtonGroup, &QButtonGroup::buttonClicked, this, &MainWindow::onDownscalingChanged);


    //placeholder -- change to the version graph and make it so a preview is only generated for the latest used "image"
    images.push_back(QtoImage(loaded));//og image (images[0]
    images.push_back(images[0]); //edited image images[1]
    previews.push_back(downscalingFunction(images[0],300,400,16));
    previews.push_back(downscalingFunction(images[1],300,400,16));


    QImage img_output = ImagetoQ(images[1]);
    img_output = img_output.scaled(
        images[0].width(),
        images[0].height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(img_output));



    //QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    //image_viewer->setPixmap(pix);
    splitter->addWidget(image_viewer);

    panelLayout->addWidget(averageDowncaling);
    panelLayout->addWidget(bicubicDownscaling);
    panelLayout->addWidget(exposureLabel);
    panelLayout->addWidget(exposureSlider);
    panelLayout->addWidget(contrastLabel);
    panelLayout->addWidget(contrastSlider);
    panelLayout->addWidget(hightlightsLabel);
    panelLayout->addWidget(hightlightsSlider);
    panelLayout->addStretch();
    splitter->addWidget(panel);
    setCentralWidget(splitter);
//ui->setupUi(this);
}

void MainWindow::onDownscalingChanged(){
    //set the function for the downscaling algorithm
    if(averageDowncaling -> isChecked()){
        downscalingFunction = downscale_image;
    }else if(bicubicDownscaling -> isChecked()){
        downscalingFunction = downscale_image_bicubic;
    }

    //placeholder until I create the version graph
    previews[0] = downscalingFunction(images[0], 300,400, 16);
    previews[1] = downscalingFunction(images[1], 300, 400, 16);
}

void MainWindow::applyExposureSlider(int slider_value, Image& edt, Image& og){
    float ex_value = slider_value/300.0f; // this value determines the amount of exposure change-- fine tune
    edt = og;
    exposure(edt, ex_value);

}


//TODO IN THE FOLLOWING METHODS:
// MAKE SURE TO CHANGE THE DATA STRUCTURE USED TO STORE THE IMAGE
void MainWindow::onExposureChange(int value){
    applyExposureSlider(value, images[1], images[0]);
    refreshDisplay();
}

void MainWindow::onExposureChangePreview(int value){
    applyExposureSlider(value, previews[1], previews[0]);
    refreshDisplayPreview();

}

void MainWindow::applyModificationSlider(std::function<void(Image&, float)> func, int slider_value, Image& edt, Image& og){
    //attempting to generalize the method to apply a change

    float value = slider_value/300.0f;
    edt = og; // this line needs to be changed, makes no sense
    func(edt, value);
}


void MainWindow::onModificationChange(std::function<void(Image&, float)> func, float value){
    applyModificationSlider(func, value, images[1], images[0]);
    refreshDisplay();
}

void MainWindow::onModificationChangePreview(std::function<void(Image&, float)> func, float value){
    applyModificationSlider(func, value, previews[1], previews[0]);
    refreshDisplayPreview();
}



void MainWindow::refreshDisplay(){
    QImage output = ImagetoQ(images[1]);
    //adding a scaling so that the image is always the same size, no matter the resolution it has. Will make this dynamic in the future
    output = output.scaled(
                       images[0].width(),
                       images[1].height(),
                       Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}

void MainWindow::refreshDisplayPreview(){
    QImage output = ImagetoQ(previews[1]);
    output = output.scaled(
        images[0].width(),
        images[0].height(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}


MainWindow::~MainWindow()
{
    delete ui;
}
