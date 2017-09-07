#pragma once
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QMainWindow>
#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QMainWindow>
#include <QDir>


#include "cvimage.h"
#include "square.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/videostab.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/stitching.hpp"
#include "opencv2/superres.hpp"
#include "opencv2/photo.hpp"
#include "opencv2/opencv_modules.hpp"

#include <vector>
#include <fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setImage(CVImage* img);
    int getFrames(cv::VideoCapture vid, int frameRate, int fileName, std::string FilePath,int topCrop, int bottomCrop);
    void start();
    void closeWin();
    void resetWindow();
    void configureWindow(int annotation ,int shape, int cursor);

signals:


private slots:

    void on_thresholdBar_sliderMoved(int position);
    void on_penColorButton_clicked();
    void on_penWidthBar_sliderMoved(int position);
    void on_undoButton_clicked();
    void on_saveButton_clicked();
    void on_skipButton_clicked();
    void on_saveButton_2_clicked();
    void on_pooButton_clicked();
    void on_seeRawButton_clicked();
    void on_seeNextButton_clicked();
    void on_seePrevButton_clicked();
    void on_thresholdBar_valueChanged(int value);
    void on_penWidthBar_valueChanged(int value);
    void increaseThresh();
    void decreaseThresh();
    void increaseBrush();
    void decreaseBrush();
    void on_ResetButton_clicked();
    void on_FrameRateButton_clicked();
    void on_brushColorButton_clicked();

private:
    int _prevThreshVal;
    int _prevBrushVal;
    Ui::MainWindow *ui;
    std::vector<cv::Mat> *_vidFrames;
    std::string _filePath;
    std::string _rawFilePath;
    std::string _posFilePath;
    std::string _negFilePath;
    std::string _posTextFile;
    std::string _negTextFile;
    std::string _rawTextFile;
    int _fileName;
    int _frameIndex;
    bool _release;
    int _frameRate;
    int _annotation;
    int _cursor;
    int _shape;


};


