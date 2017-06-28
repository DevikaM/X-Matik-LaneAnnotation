#include "mainwindow.h"
#include "firstwindow.h"
#include "cvimage.h"
#include <QApplication>
#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QMainWindow>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FirstWindow window;
    window.show();

    return app.exec();


}
