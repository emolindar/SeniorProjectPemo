#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "src/Image.h"
#include "src/imageconverter.h"
#include "src/algorithms/exposure.h"
#include "src/algorithms/contrast.h"
#include "src/algorithms/highlights.h"
#include "src/algorithms/shadows.h"
#include <QSlider>


/*TODO

-> add a way to export the image at the end

->  start thinking about optimizing all the functions, most just have a O(n^2)
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

    Image img = QtoImage(loaded);
    graph = new VersionGraph(img);

    //testing exposure
    exposureSlider->setRange(-300, 300);
    exposureSlider->setValue(0);

    //this makes it so that the function onExposureChange is called when the value of the slider is changed
    connect(exposureSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(exposure, value); });

    connect(exposureSlider, &QSlider::sliderReleased, this, [this]() {onModificationChange("exposure", exposure, exposureSlider->value());});
    //^ need to add lambda function which can grab the exposure slider value and then call the on exposure change function

    //attempting to add the contrast slider with the generalized function
    contrastSlider->setRange(-300, 300);
    contrastSlider->setValue(0);

    //connecting the contrastSlider with the modified function but with the generalized method for applying modifications
    connect(contrastSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(contrast, value); });
    //doing the same for the contrast preview
    connect(contrastSlider, &QSlider::sliderReleased, this, [this]() {onModificationChange("contrast",contrast,contrastSlider->value());});


    //highlights
    hightlightsSlider->setRange(-300, 300);
    hightlightsSlider->setValue(0);

    connect(hightlightsSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(highlights, value);});
    connect(hightlightsSlider, &QSlider::sliderReleased, this, [this](){onModificationChange("highlights", highlights, hightlightsSlider->value());} );

    //shadows
    shadowsSlider->setRange(-300, 300);
    shadowsSlider->setValue(0);

    connect(shadowsSlider, &QSlider::valueChanged, this, [this](int value){onModificationChangePreview(shadows, value);});
    connect(shadowsSlider, &QSlider::sliderReleased, this, [this]() {onModificationChange("shadows", shadows, shadowsSlider->value());});

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


    QImage img_output = ImagetoQ(graph->getWorkingImage());
    img_output = img_output.scaled(
        img.width(),
        img.height(),
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
    panelLayout->addWidget(shadowsLabel);
    panelLayout->addWidget(shadowsSlider);
    panelLayout->addStretch();
    splitter->addWidget(panel);
    setCentralWidget(splitter);
//ui->setupUi(this);
}

void MainWindow::onDownscalingChanged(){
    //set the function for the downscaling algorithm


    //CURRENTLY USELESS, CREATE MECHANISM IN VERSIONGRAPH TO SUPPORT THIS
}

void MainWindow::onModificationChange(std::string name, std::function<void(Image&, float)> func, float value){
    graph->addNode(name, func, value/300.0f);
    refreshDisplay();
    graph->refreshPreview();
}

void MainWindow::onModificationChangePreview(std::function<void(Image&, float)> func, float value){
    graph -> updatePreview(func, value/300.0f);
    refreshDisplayPreview();
    graph -> refreshPreview();
}

void MainWindow::refreshDisplay(){
    QImage output = ImagetoQ(graph->getWorkingImage());
    //adding a scaling so that the image is always the same size, no matter the resolution it has. Will make this dynamic in the future
    output = output.scaled(
                       graph->getRootImage().width(),
                       graph->getRootImage().height(),
                       Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}

void MainWindow::refreshDisplayPreview(){
    QImage output = ImagetoQ(graph-> getPreview());
    output = output.scaled(
        graph->getRootImage().width(),
        graph->getRootImage().height(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}


MainWindow::~MainWindow()
{
    delete ui;
}
