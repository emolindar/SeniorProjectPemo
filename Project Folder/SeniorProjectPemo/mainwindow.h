#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QListView>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QSlider>
#include "src/Image.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QSplitter *splitter = new QSplitter();
    QListView *listview = new QListView;
    QLabel *image_viewer = new QLabel;

    QSlider *exposureSlider = new QSlider(Qt::Horizontal);

    Ui::MainWindow *ui;

    //the data structure that will hold the image versions will go here, going with vector for now, for ease
    vector<Image> images;


    //my own functions (which should count towards the algorithms)

    //refresh function

    void refreshDisplay();
    //exposure
    void applyExposureSlider(int slider_value, Image& etd, Image& og);

    //not fully sure about this, but in order to "connect" the updating slider with the function, I need to add it to its own group like this
private slots:
    void onExposureChange(int value);


};
#endif // MAINWINDOW_H
