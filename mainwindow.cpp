#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QAction *undoAction = new QAction(this);
    undoAction->setShortcut(Qt::Key_Z| Qt::CTRL);
    connect(undoAction, SIGNAL(triggered()), this, SLOT(on_undoButton_clicked()));
    this->addAction(undoAction);

    QAction *saveAction = new QAction(this);
    saveAction->setShortcut(Qt::Key_Space);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(on_saveButton_clicked()));
    this->addAction(saveAction);

    QAction *skipAction = new QAction(this);
    skipAction->setShortcut(Qt::Key_S| Qt::CTRL);
    connect(skipAction, SIGNAL(triggered()), this, SLOT(on_saveButton_2_clicked()));
    this->addAction(skipAction);

    QAction *rawAction = new QAction(this);
    rawAction->setShortcut(Qt::Key_R| Qt::CTRL);
    connect(rawAction, SIGNAL(triggered()), this, SLOT(on_seeRawButton_clicked()));
    this->addAction(rawAction);

    QAction *negativeAction = new QAction(this);
    negativeAction->setShortcut(Qt::Key_N);
    connect(negativeAction, SIGNAL(triggered()), this, SLOT(on_skipButton_clicked()));
    this->addAction(negativeAction);

    QAction *increaseThreshAction = new QAction(this);
    increaseThreshAction->setShortcut(Qt::Key_W);
    connect(increaseThreshAction, SIGNAL(triggered()), this, SLOT(increaseThresh()));
    ui->thresholdBar->addAction(increaseThreshAction);

    QAction *decreaseThreshAction = new QAction(this);
    decreaseThreshAction->setShortcut(Qt::Key_S);
    connect(decreaseThreshAction, SIGNAL(triggered()), this, SLOT(decreaseThresh()));
    this->addAction(decreaseThreshAction);

    QAction *increaseBrushAction = new QAction(this);
    increaseBrushAction->setShortcut(Qt::Key_D);
    connect(increaseBrushAction, SIGNAL(triggered()), this, SLOT(increaseBrush()));
    this->addAction(increaseBrushAction);

    QAction *decreaseBrushAction = new QAction(this);
    decreaseBrushAction->setShortcut(Qt::Key_A);
    connect(decreaseBrushAction, SIGNAL(triggered()), this, SLOT(decreaseBrush()));
    this->addAction(decreaseBrushAction);


    _frameRate = 1;
    _vidFrames = new std::vector<cv::Mat>;
    _frameIndex = 0;
    _release = false;
    _prevThreshVal = ui->thresholdBar->value();
    _prevBrushVal = ui->penWidthBar->value();
    CVImage* imageWidget = new CVImage();
    imageWidget->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    cv::Mat temp(130, 220, CV_8UC3, cv::Scalar(255,255,255));
    imageWidget->showImage(temp);
    QRectF rect(110.0, 25.0, 25.0, 25.0);
    imageWidget->drawCircle(rect,false,true);
    ui->mdiArea_2->addSubWindow(imageWidget,Qt::FramelessWindowHint);
    /*CANT BE USED WITH DYNAMIC FRAME RATE*/
    ui->seeNextButton->hide();
    ui->seePrevButton->hide();

}


void MainWindow::ResetWindow(){

    _vidFrames = new std::vector<cv::Mat>;
    _frameIndex = 0;
    _release = false;
    _prevThreshVal = ui->thresholdBar->value();
    _prevBrushVal = ui->penWidthBar->value();

}

void MainWindow::increaseThresh()
{
    ui->thresholdBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}
void MainWindow::decreaseThresh()
{
    ui->thresholdBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void MainWindow::increaseBrush()
{
    ui->penWidthBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}
void MainWindow::decreaseBrush()
{
    ui->penWidthBar->triggerAction(QAbstractSlider::SliderSingleStepSub);
}
int MainWindow::getFrames(cv::VideoCapture vid, int frameRate, int fileName,std::string FilePath,int topCrop, int bottomCrop)
{
    this->ResetWindow();
    _fileName = fileName;
    _filePath = FilePath;
    _frameRate = frameRate;
    ui->FrameRateTextBox->text() = _frameRate;
    /*************************************/
    _posFilePath = QDir::cleanPath(QString::fromStdString(FilePath) + QDir::separator() + "pos").toStdString();
    _negFilePath =QDir::cleanPath(QString::fromStdString(FilePath) + QDir::separator() + "neg").toStdString();
    _rawFilePath = QDir::cleanPath(QString::fromStdString(FilePath) + QDir::separator() + "raw").toStdString();

    if(!QDir(QString::fromStdString(_posFilePath)).exists())
        QDir(QString::fromStdString(_posFilePath)).mkpath(".");
    if(!QDir(QString::fromStdString(_negFilePath)).exists())
        QDir(QString::fromStdString(_negFilePath)).mkpath(".");
    if(!QDir(QString::fromStdString(_rawFilePath)).exists())
        QDir(QString::fromStdString(_rawFilePath)).mkpath(".");

    _posTextFile = (QDir(QString::fromStdString(_posFilePath)).filePath("pos.txt")).toStdString();
    _negTextFile = (QDir(QString::fromStdString(_negFilePath)).filePath("neg.txt")).toStdString();
    _rawTextFile = (QDir(QString::fromStdString(_rawFilePath)).filePath("raw.txt")).toStdString();

    /* FOR OSX
    _posTextFile = FilePath + "/pos.txt";
    _negTextFile = FilePath + "/neg.txt";
    _rawTextFile = FilePath + "/raw.txt";*/

    /* FOR WINDOWS
    _posTextFile = FilePath + "\\pos.txt";
    _negTextFile = FilePath + "\\neg.txt";
    _rawTextFile = FilePath + "\\raw.txt";*/

    /************************************/

    if(vid.isOpened()){
        try
        {
            int totalVidFrame = (int) vid.get(CV_CAP_PROP_FRAME_COUNT);
            //int grabFrame = 0;

            for(int frameIndex = 0; frameIndex < totalVidFrame; frameIndex++)
            {
               /* if(grabFrame == 0)
                {
                    cv::Mat tempFrame;
                    vid.set(cv::CAP_PROP_POS_FRAMES, frameIndex);
                    vid.grab();
                    vid.retrieve(tempFrame);
                    _vidFrames->push_back(tempFrame);
                    fileName++;
                }else if(grabFrame == frameRate)
                {
                    grabFrame = -1;
                }
                grabFrame++;*/

                cv::Mat tempFrame, cropped;
                vid.set(cv::CAP_PROP_POS_FRAMES, frameIndex);
                vid.grab();
                vid.retrieve(tempFrame);
                cv::Rect size(0,topCrop, tempFrame.size().width,((tempFrame.size().height-topCrop)-bottomCrop));
                cropped = cv::Mat(tempFrame, size).clone();
                _vidFrames->push_back(cropped);
                fileName++;
            }

        }
        catch(cv::Exception e)
        {
            /*QMessageBox messageBox;
            messageBox.critical(0,"Error at MainWindow->getFrames()",e.msg);
            messageBox.setFixedSize(500,200);
            exit(0);*/
        }

    }

    return fileName;

}

void MainWindow::start()
{
    CVImage* image = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
    image->showImage(_vidFrames->at(_frameIndex));
    //_frameIndex++;
    //_frameIndex+=_frameRate;
    ui->mdiArea->activeSubWindow()->setWidget(image);
}

void MainWindow::setImage(CVImage *img)
{
    ui->mdiArea->addSubWindow(img,Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_thresholdBar_sliderMoved(int position)
{
    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
    temp->applyThreshold(position);
    ui->mdiArea->activeSubWindow()->setWidget(temp);
    _prevThreshVal = ui->thresholdBar->value();
}

void MainWindow::on_penWidthBar_sliderMoved(int position)
{
    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
    temp->setWidth(position);
    ui->mdiArea->activeSubWindow()->setWidget(temp);
    temp = (CVImage*) ui->mdiArea_2->activeSubWindow()->widget();
    QRectF rect(110.0, 25.0, position, position);
    temp->drawCircle(rect);
    ui->mdiArea_2->activeSubWindow()->setWidget(temp);
    _prevBrushVal = ui->penWidthBar->value();
}

void MainWindow::on_penColorButton_clicked()
{
     QColor newColor = QColorDialog::getColor();
     CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
     temp->setColor(newColor);
     ui->mdiArea->activeSubWindow()->setWidget(temp);
}

void MainWindow::on_undoButton_clicked()
{
 CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
 temp->undo();
}

void MainWindow::on_saveButton_clicked()
{

    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();

    /*********************************/
    std::fstream pos;
    pos.open(_posTextFile, std::fstream::app | std::fstream::out );

    std::fstream raw;
    raw.open(_rawTextFile, std::fstream::app | std::fstream::out );

    std::string fullFilePath = QDir(QString::fromStdString(_posFilePath)).filePath(QString::fromStdString(std::to_string(_fileName) + "p.png")).toStdString();
    pos << (QDir("pos").filePath(QString::fromStdString(std::to_string(_fileName)) + "p.png\n")).toStdString();

    temp->save(fullFilePath);

    fullFilePath = QDir(QString::fromStdString(_rawFilePath)).filePath(QString::fromStdString(std::to_string(_fileName) + "r.png")).toStdString();
    raw << (QDir("raw").filePath(QString::fromStdString(std::to_string(_fileName)) + "r.png\n")).toStdString();

    temp->save(fullFilePath,true);

    //std::string fullFilePath = _filePath + "/pos/" + std::to_string(_fileName) + "p.png";
    //pos << "pos/" + std::to_string(_fileName) + "p.png\r";

    //std::string fullFilePath = filePath + "\\pos\\" + std::to_string(_fileName) + "p.png";
    //pos << "pos\\" + std::to_string(_fileName) + "p.png";
    /*********************************/

    _fileName++;

    if(_frameIndex + _frameRate < _vidFrames->size())
    {
        _frameIndex+=_frameRate;
         temp->showImage(_vidFrames->at(_frameIndex));
         ui->mdiArea->activeSubWindow()->setWidget(temp);
         if (ui->thresholdBar->value() > 1)
             this->on_thresholdBar_sliderMoved(ui->thresholdBar->value());
     }
    else{
        cv::Mat tempMat(500, 880, CV_8UC3, cv::Scalar(0,0,0));
        temp->showImage(tempMat);
        ui->mdiArea->activeSubWindow()->setWidget(temp);
        this->close();
    }
    pos.close();
    raw.close();

}

void MainWindow::on_skipButton_clicked()
{
    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
    if(_frameIndex + _frameRate  < _vidFrames->size())
    {
        _frameIndex+=_frameRate;
         temp->showImage(_vidFrames->at(_frameIndex));
         //_frameIndex++;
         //_vidFrames->erase(_vidFrames->begin());
         ui->mdiArea->activeSubWindow()->setWidget(temp);
         if (ui->thresholdBar->value() > 1)
             this->on_thresholdBar_sliderMoved(ui->thresholdBar->value());
     }
    else{
        cv::Mat tempMat(500, 880, CV_8UC3, cv::Scalar(0,0,0));
        temp->showImage(tempMat);
        ui->mdiArea->activeSubWindow()->setWidget(temp);
        this->close();
    }

}

//SKIP BUTTON
void MainWindow::on_saveButton_2_clicked()
{
    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();

    /*********************************/    
    std::fstream neg;
    neg.open(_negTextFile, std::fstream::app | std::fstream::out);

    std::fstream raw;
    raw.open(_rawTextFile, std::fstream::app | std::fstream::out );

    std::string fullFilePath = QDir(QString::fromStdString(_negFilePath)).filePath(QString::fromStdString(std::to_string(_fileName) + "n.png")).toStdString();
    neg << (QDir("neg").filePath(QString::fromStdString(std::to_string(_fileName)) + "n.png\n")).toStdString();

    temp->saveNeg(fullFilePath);

    fullFilePath = QDir(QString::fromStdString(_rawFilePath)).filePath(QString::fromStdString(std::to_string(_fileName) + "r.png")).toStdString();
    raw << (QDir("raw").filePath(QString::fromStdString(std::to_string(_fileName)) + "r.png\n")).toStdString();

    temp->save(fullFilePath,true);

    neg.close();
    raw.close();

    //std::string fullFilePath = _filePath + "/neg/" + std::to_string(_fileName) + "n.png";
    // neg << "neg/" + std::to_string(_fileName) + "n.png\r";

    //std::string fullFilePath = filePath + "\\neg\\" + std::to_string(_fileName) + "n.png";
    //pos << "neg\\" + std::to_string(_fileName) + "n.png";
    /*********************************/

    _fileName++;

    if(_frameIndex + _frameRate < _vidFrames->size())
    {
        _frameIndex+=_frameRate;
         temp->showImage(_vidFrames->at(_frameIndex));
         //_frameIndex++;
         //_vidFrames->erase(_vidFrames->begin());
         ui->mdiArea->activeSubWindow()->setWidget(temp);
         if (ui->thresholdBar->value() > 1)
             this->on_thresholdBar_sliderMoved(ui->thresholdBar->value());

     }
    else{
        cv::Mat tempMat(500, 880, CV_8UC3, cv::Scalar(0,0,0));
        temp->showImage(tempMat);
        ui->mdiArea->activeSubWindow()->setWidget(temp);
        this->close();
    }

    //ui->thresholdBar->setValue(1);

}
void MainWindow::on_pooButton_clicked()
{
    if(!_release){
        if(_vidFrames->size() >=  1 && _frameIndex >= 1){
            CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
            cv::Mat tmpFrame = temp->_tmpRaw, colorFrame;
            cv::applyColorMap(tmpFrame, colorFrame, cv::COLORMAP_RAINBOW);
            cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
           // cv::resizeWindow("image", 200,400);
            cv::imshow("image",colorFrame);
            cv::waitKey(1);
            _release = true;
        }
    }else {
        cvDestroyAllWindows();

        _release = false;
    }
}
void MainWindow::on_seeRawButton_clicked()
{
    if(!_release){
        if(_vidFrames->size() >=  1 && _frameIndex >= 1){
            CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();

            //CVImage img;
            //temp->showImage(temp->_tmpRaw);
            //temp->show();
            cv::Mat tmpFrame;// = temp->_tmpRaw;
            cv::resize(temp->_tmpRaw, tmpFrame, cv::Size(800,500));
            cv::namedWindow("raw", cv::WINDOW_KEEPRATIO);
            //cv::resizeWindow("raw", 50,30);
            cv::imshow("raw",tmpFrame);
            cv::waitKey(1);
            _release = true;
        }
    }else{
        cvDestroyAllWindows();
        _release = false;
    }
}
/*CANT BE USED WITH DYNAMIC FRAME RATE*/
void MainWindow::on_seeNextButton_clicked()
{
    if(!_release){
        if(_vidFrames->size() > 1 && _frameIndex+1 <= _vidFrames->size()){
            CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
            cv::Mat tmpFrame = _vidFrames->at(_frameIndex+1);
            cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
           // cv::resizeWindow("image", 200,400);
            cv::imshow("image",tmpFrame);
            cv::waitKey(1);
            _release = true;
        }
    }
    else{
         cvDestroyAllWindows();
         _release = false;
    }
}
/*CANT BE USED WITH DYNAMIC FRAME RATE*/
void MainWindow::on_seePrevButton_clicked()
{
  if(!_release){
    if(_vidFrames->size() > 1 && _frameIndex > 1 && _frameIndex <= _vidFrames->size() ){
        std::cout << _vidFrames->size() ;
        CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
        cv::Mat tmpFrame = _vidFrames->at(_frameIndex-1);
        cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
        //cv::resizeWindow("image", 200,400);
        cv::imshow("image",tmpFrame);
        cv::waitKey(1);
        _release = true;
    }
  }else{
    cvDestroyAllWindows();
    _release = false;
      }
}
void MainWindow::on_thresholdBar_valueChanged(int value)
{
    on_thresholdBar_sliderMoved(value);
   _prevThreshVal = ui->thresholdBar->value();
}

void MainWindow::on_penWidthBar_valueChanged(int value)
{
    on_penWidthBar_sliderMoved(value);
    _prevBrushVal = ui->penWidthBar->value();
}

void MainWindow::on_ResetButton_clicked()
{
    CVImage* temp = (CVImage*) ui->mdiArea->activeSubWindow()->widget();
    temp->reset();
    ui->mdiArea->activeSubWindow()->setWidget(temp);
}

void MainWindow::on_FrameRateButton_clicked()
{
    if (ui->FrameRateTextBox->text().toStdString() != "")
        _frameRate = atoi((ui->FrameRateTextBox->text().toStdString()).c_str());
}
