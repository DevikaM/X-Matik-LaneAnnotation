#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QMainWindow>
#include <QtGui>
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
#include "mainwindow.h"
#include "cvimage.h"
#include "square.h"
#include "configuration.h"
#include <string>
#include <map>

namespace Ui {
class FirstWindow;
}

class FirstWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FirstWindow(QWidget *parent = 0);
    ~FirstWindow();

private slots:
    void on_SubmitButton_clicked();

    void on_annotationComboBox_currentIndexChanged(int index);

private:
    Ui::FirstWindow *ui;
    MainWindow *window;

};

#endif // FIRSTWINDOW_H
