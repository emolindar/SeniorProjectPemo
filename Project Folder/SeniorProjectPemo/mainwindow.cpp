#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "src/Image.h"
#include "src/imageconverter.h"

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

    //testing exposure
    exposure(img, 2.0f);


    QImage img_output = ImagetoQ(img);
    image_viewer->setPixmap(QPixmap::fromImage(img_output));
    


    //QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    //image_viewer->setPixmap(pix);
    splitter->addWidget(image_viewer);
    splitter->addWidget(listview);
    setCentralWidget(splitter);
//ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
