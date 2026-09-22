#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    image_viewer->setPixmap(pix);
    splitter->addWidget(image_viewer);
    splitter->addWidget(listview);
    setCentralWidget(splitter);
//ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
