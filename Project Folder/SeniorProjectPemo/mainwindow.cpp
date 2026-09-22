#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    splitter->addWidget(listview);
    splitter->addWidget(listview2);
    w.setCentralWidget(splitter);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
