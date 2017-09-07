#include "firstwindow.h"
#include "ui_firstwindow.h"

FirstWindow::FirstWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FirstWindow)
{
    ui->setupUi(this);
    window = new MainWindow(this);

    for(auto const& annotation : Configuration::getAnnotation())
        ui->annotationComboBox->addItem(QString::fromStdString(annotation.second), Qt::DisplayRole);
    for(auto const& shape : Configuration::getShapes())
        ui->shapeComboBox->addItem(QString::fromStdString(shape.second), Qt::DisplayRole);
    for(auto const& cursor: Configuration::getCursors())
        ui->cursorComboBox->addItem(QString::fromStdString(cursor.second), Qt::DisplayRole);

}

FirstWindow::~FirstWindow()
{
    delete ui;
}

void FirstWindow::on_SubmitButton_clicked()
{
    std::string Path = ui->lineEdit->text().toStdString();
    std::string OutDir = ui->out_text_dir->text().toStdString();
    std::string File = ui->file_name->text().toStdString();
    std::string FrameRate = ui->frame_rate->text().toStdString();
    std::string CropTop = ui->crop_top->text().toStdString();
    std::string CropBottom = ui->crop_bottom->text().toStdString();


    int frameRate = atoi((FrameRate).c_str());
    int fileName = atoi((File).c_str());
    int topCrop = atoi((CropTop).c_str());
    int bottomCrop = atoi((CropBottom).c_str());
    int annotation = ui->annotationComboBox->currentIndex();
    int shape = ui->shapeComboBox->currentIndex();
    int cursor = ui->cursorComboBox->currentIndex();
    std::string FileName = Path;

    if((topCrop > 260 || topCrop < 0) || (bottomCrop > 260 || bottomCrop < 0))
    {
        QMessageBox messageBox;
                    messageBox.critical(0,"Error","Crop values must be between 0 and 260");
                    messageBox.setFixedSize(500,200);
           return;
    }

    if(FileName.find("/") == 0)
        FileName = FileName.substr(FileName.find_last_of("/")+1, FileName.length() - FileName.find_last_of("/"));

    else if(FileName.find("\\") == 0)
        FileName = FileName.substr(FileName.find_last_of("\\")+1, FileName.length() - FileName.find_last_of("\\"));

    if(!QFile(QString::fromStdString(Path)).exists()){
        QMessageBox messageBox;
                    messageBox.critical(0,"Error","Video path not valid");
                    messageBox.setFixedSize(500,200);
           return;
    }

    cv::VideoCapture video(Path);

    ui->textEdit->insertPlainText(QString::fromStdString("Loading: " + FileName + "\n"));

    window->configureWindow(annotation,shape, cursor);
    int Frames = window->getFrames(video, frameRate, fileName, OutDir,topCrop,bottomCrop);
    window->show();
    window->start();

    ui->textEdit->insertPlainText(QString::fromStdString(std::to_string(Frames) + " Frames Selected from " + FileName + "\n"));
}

void FirstWindow::on_annotationComboBox_currentIndexChanged(int index)
{
    ui->shapeComboBox->setCurrentIndex(index);
    ui->cursorComboBox->setCurrentIndex(index);
}
