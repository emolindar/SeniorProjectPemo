#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "src/Image.h"
#include "src/imageconverter.h"
#include "src/algorithms/exposure.h"
#include "src/algorithms/contrast.h"
#include "src/algorithms/highlights.h"
#include "src/algorithms/shadows.h"
#include <QSlider>


#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>


/*TODO

-> add a way to export the image at the end
-> fix the way the downscaling is incorporated -- with high resolution images, app lags too much and it isn't shown properly
*/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


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




    //setting starting values to the History Label
    historyTextBox -> setReadOnly(true);
    historyTextBox -> setPlainText("");

    //QPixmap pix("/Users/emolindar/AUBG/Senior Project/Project/Project Folder/SeniorProjectPemo/images/images.jpeg");
    //image_viewer->setPixmap(pix);
    splitterEditor->addWidget(image_viewer);

    adjustmentsPanelLayout->addWidget(averageDowncaling);
    adjustmentsPanelLayout->addWidget(bicubicDownscaling);
    adjustmentsPanelLayout->addWidget(exposureLabel);
    adjustmentsPanelLayout->addWidget(exposureSlider);
    adjustmentsPanelLayout->addWidget(contrastLabel);
    adjustmentsPanelLayout->addWidget(contrastSlider);
    adjustmentsPanelLayout->addWidget(hightlightsLabel);
    adjustmentsPanelLayout->addWidget(hightlightsSlider);
    adjustmentsPanelLayout->addWidget(shadowsLabel);
    adjustmentsPanelLayout->addWidget(shadowsSlider);

    adjustmentsPanelLayout->addWidget(historyTextBox, 1);
    adjustmentsPanelLayout->addStretch();

    splitterEditor->addWidget(adjustmentsPanel);


    //setting things up for the graph tab

    graphTextBox->setReadOnly(true);
    graphTextBox->setPlainText("Import Image to see Graph");

    graphPanelLayout -> addWidget(graphTextBox);
    graphPanelLayout -> addWidget(branchNameInput);
    graphPanelLayout -> addWidget(branchButton);
    connect(branchButton, &QPushButton::clicked, this,&MainWindow::branchButtonPressed);

    graphPanelLayout->addWidget(branchesBox);
    graphPanelLayout->addWidget(checkoutButton);
    connect(checkoutButton, &QPushButton::clicked, this, &MainWindow::checkoutButtonPressed);

    //setting things up for the Image import tab
    imageInfoLabel-> setAlignment(Qt::AlignCenter);
    previewLabel -> setAlignment(Qt::AlignCenter);
    imageSelectLayout -> addWidget(importButton);

    connect(importButton, &QPushButton::clicked, this, &MainWindow::importImage);
    imageSelectLayout->addWidget(imageInfoLabel);
    imageSelectLayout->addWidget(previewLabel,1);

    adjustmentsPanel -> setEnabled(false);
    QLabel *preveiwDimensionsLabel = new QLabel("Change the preview image dimensions(if application lagging or you desire different results);");
    imageSelectLayout -> addWidget(preveiwDimensionsLabel);
    QHBoxLayout *previewWidthInputLayout = new QHBoxLayout();
    previewWidthInputLayout -> addWidget(previewWidthInputLabel);
    previewWidthInputLayout -> addWidget(previewWidthInput);

    imageSelectLayout -> addLayout(previewWidthInputLayout);


    imageSelectLayout -> addWidget(applyPreviewDimensions);

    connect(applyPreviewDimensions, &QPushButton::clicked, this, [this]() {
        if(graph){
            int w = previewWidthInput -> text().toInt();


            if(w>0 ){
                graph -> setPreviewDimensions(w);

                graph -> getWorkingImage();

            }
        }
    });
    //structuring all of the widgets into tabs
    tabs->addTab(imageSelectPage, "Image Select");
    tabs->addTab(splitterEditor, "Editor");
    tabs->addTab(graphPanel, "Version Graph");


    setCentralWidget(tabs);
}

void MainWindow::onDownscalingChanged(){
    //set the function for the downscaling algorithm


    //CURRENTLY USELESS, CREATE MECHANISM IN VERSIONGRAPH TO SUPPORT THIS
}

void MainWindow::onModificationChange(std::string name, std::function<void(Image&, float)> func, float value){
    graph->addNode(name, func, value/1200.0f);
    refreshDisplay();
}

void MainWindow::onModificationChangePreview(std::function<void(Image&, float)> func, float value){
    graph -> updatePreview(func, value/1200.0f);
    refreshDisplayPreview();
}

void MainWindow::refreshDisplay(){
    QImage output = ImagetoQ(graph->getWorkingImage());
    //adding a scaling so that the image is always the same size, no matter the resolution it has. Will make this dynamic in the future
    output = output.scaled(
                       800,
                       800,
                       Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));

    historyTextBox -> setPlainText(QString::fromStdString(graph -> history()));
    RefreshGraphVisual();
}

void MainWindow::refreshDisplayPreview(){
    QImage output = ImagetoQ(graph-> getPreview());
    output = output.scaled(
        800,
        800,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);
    image_viewer->setPixmap(QPixmap::fromImage(output));
}

void MainWindow::importImage(){
    /*function that opens a file explorer page and gets the path of a selected image.
    * after the image is received, it is shown in a preview window
    */
    QString path = QFileDialog::getOpenFileName(this, "Import Image", QString());
    if(path.isEmpty()) return;

    QImageReader reader(path);
    QImage  loaded = reader.read();

    //error checking
    if(loaded.isNull()){
        QMessageBox::warning(this, "import failed", reader.errorString());
        return;
    }

    Image img = QtoImage(loaded);
    delete graph;
    graph = new VersionGraph(img);

    refreshDisplay();


    imageInfoLabel -> setText(QString::fromStdString(QFileInfo(path).fileName().toStdString() + "\n" + to_string(loaded.width()) + "\n" + to_string(loaded.height())));
    previewLabel->setPixmap(QPixmap::fromImage(loaded.scaled(400,300, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
    adjustmentsPanel -> setEnabled(true);
}

void MainWindow::RefreshGraphVisual(){
    graphTextBox -> setPlainText(QString::fromStdString("Version Graph Visualizer\n\n" + graph -> vizualize()));
    refreshBranchesBox();
}

void MainWindow::branchButtonPressed(){
    graph->createBranch(branchNameInput->text().toStdString());
    branchNameInput->clear();
    RefreshGraphVisual();
}

void MainWindow::checkoutButtonPressed(){
    graph->checkout(branchesBox->currentText().toStdString());
    refreshDisplay();
}

void MainWindow::refreshBranchesBox(){
    branchesBox->clear();
    std::vector<std::string> branches = graph->getBranchNames();

    for(int i=0; i<branches.size();i++){
        branchesBox ->addItem(QString::fromStdString(branches[i]));
    }

    branchesBox->setCurrentText(QString::fromStdString(graph->getCurrentBranchName()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
