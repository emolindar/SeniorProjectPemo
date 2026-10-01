#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "src/Image.h"
#include "src/imageconverter.h"
#include "src/algorithms/exposure.h"
#include <QSlider>
/*TODO
1. add a deep copy for Image
2. add a slider to change the exposure value  - added
3. start working on the image version graph
4. add a refresh function, which refreshes the image every time a change is made ( i.e. the slider is moved)
5. add a way to export the image at the end
6. start thinking about optimizing all the functions, most just have a O(n^2)
*/



void applyExposureSlider(int slider_value, Image& etd, Image& og){
    float ex_value = slider_value/100.0f;
    etd = og;
    exposure(etd, ex_value);

}

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

    Image original_img = QtoImage(loaded);
    Image edited_img = original_img;

    //testing exposure
    exposure->setRange(-300, 300);
    exposure->setValue(0);
    applyExposureSlider(exposure->value(), edited_img, original_img);


    QImage img_output = ImagetoQ(edited_img);
    image_viewer->setPixmap(QPixmap::fromImage(img_output));
    


    //QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    //image_viewer->setPixmap(pix);
    splitter->addWidget(image_viewer);
    splitter->addWidget(listview);
    splitter->addWidget(exposure);
    setCentralWidget(splitter);
//ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
