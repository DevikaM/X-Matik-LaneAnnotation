#include "firstwindow.h"
#include "ui_firstwindow.h"

FirstWindow::FirstWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FirstWindow)
{
    ui->setupUi(this);
    window = new MainWindow(this);

/*
    CVImage* imageWidget = new CVImage();
    imageWidget->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    cv::Mat temp(500, 880, CV_8UC3, cv::Scalar(0,0,0));
    imageWidget->showImage(temp);
    window->setImage(imageWidget);
    window->show();
*/
}

FirstWindow::~FirstWindow()
{
    delete ui;
}

void FirstWindow::on_SubmitButton_clicked()
{
    //window->ResetWindow();

    std::string Path = ui->lineEdit->text().toStdString();
    std::string OutDir = ui->out_text_dir->text().toStdString();
    std::string File = ui->file_name->text().toStdString();
    std::string FrameRate = ui->frame_rate->text().toStdString();

    int frameRate = atoi((FrameRate).c_str());
    int fileName = atoi((File).c_str());
    std::string FileName = Path;

    if(FileName.find("/") == 0)
        FileName = FileName.substr(FileName.find_last_of("/")+1, FileName.length() - FileName.find_last_of("/"));

    else if(FileName.find("\\") == 0)
        FileName = FileName.substr(FileName.find_last_of("\\")+1, FileName.length() - FileName.find_last_of("\\"));

    cv::VideoCapture video(Path);
    ui->textEdit->insertPlainText(QString::fromStdString("Loading: " + FileName + "\n"));

    int Frames = window->getFrames(video, frameRate, fileName, OutDir);
    CVImage* imageWidget = new CVImage();
    imageWidget->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    cv::Mat temp(500, 880, CV_8UC3, cv::Scalar(0,0,0));
    imageWidget->showImage(temp);
    window->setImage(imageWidget);
    window->show();
    window->start();
    ui->textEdit->insertPlainText(QString::fromStdString(std::to_string(Frames) + " Frames Selected from " + FileName + "\n"));
}
