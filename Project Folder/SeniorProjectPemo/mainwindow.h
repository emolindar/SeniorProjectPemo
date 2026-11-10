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
#include "src/versionGraph.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>


//choosing which downscaling algorithm is used
#include <QRadioButton>
#include <QButtonGroup>
#include <functional>

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
    QTabWidget *tabs = new QTabWidget();


//Editor tab ---------------------------------------------------------------
    QSplitter *splitterEditor = new QSplitter();
    QListView *listview = new QListView;
    QLabel *image_viewer = new QLabel;
    QWidget *adjustmentsPanel = new QWidget();
    QVBoxLayout *adjustmentsPanelLayout = new QVBoxLayout(adjustmentsPanel);


    //widgets needed in order to choose which downscaling algorithm is used
    QRadioButton *averageDowncaling;
    QRadioButton *bicubicDownscaling;
    QButtonGroup *downscalingButtonGroup;


    QLabel *exposureLabel = new QLabel("Exposure", this);
    QSlider *exposureSlider = new QSlider(Qt::Horizontal);
    QLabel *contrastLabel = new QLabel("Contrast", this);
    QSlider *contrastSlider = new QSlider(Qt::Horizontal);
    QLabel *hightlightsLabel = new QLabel("Highlights", this);
    QSlider *hightlightsSlider = new QSlider(Qt::Horizontal);
    QLabel *shadowsLabel = new QLabel("Shadows",this);
    QSlider *shadowsSlider = new QSlider(Qt::Horizontal);
    QPlainTextEdit *historyTextBox = new QPlainTextEdit();

    //my own functions (which should count towards the algorithms)

    //refresh function
    void refreshDisplay();
    void refreshDisplayPreview();

    std::function<Image(Image&,int,int,int)> downscalingFunction;

    void onModificationChange(std::string name, std::function<void (Image &, float)> func, float value);
    void onModificationChangePreview(std::function<void (Image&, float)> func, float value);

    void onDownscalingChanged();

//GraphVersion tab ---------------------------------------------------------------------
    QWidget *graphPanel = new QWidget();
    QVBoxLayout *graphPanelLayout = new QVBoxLayout(graphPanel);
    QPlainTextEdit *graphTextBox = new QPlainTextEdit();


//Image Select tab ------------------------------------------------------------------------
    QWidget *imageSelectPage = new QWidget();
    QVBoxLayout *imageSelectLayout = new QVBoxLayout(imageSelectPage);
    QPushButton *importButton = new QPushButton("Import Image");
    QLabel *imageInfoLabel = new QLabel("No Image Loaded");
    QLabel *previewLabel = new QLabel();


    QLabel *previewWidthInputLabel = new QLabel("Desired preview width: ");
    QLineEdit *previewWidthInput = new QLineEdit("400");
    QLabel *previewHeightInputLabel = new QLabel("Desired preview height: ");
    QLineEdit *previewHeightInput = new QLineEdit("200");
    QPushButton *applyPreviewDimensions = new QPushButton("Apply new preview dimensions.");


    Ui::MainWindow *ui;

    //version graph structure, replacing placeholder vector structure from before
    VersionGraph* graph = nullptr;


private slots:
    //this is necessary for the importing of images
    void importImage();
};
#endif // MAINWINDOW_H
