
QT       += core gui widgets#include <QWidget>

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaneAnnotation
TEMPLATE = app

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_video  -lopencv_videoio  -lopencv_videostab

SOURCES += main.cpp\
        mainwindow.cpp \
    cvimage.cpp \
    firstwindow.cpp \
    thresholdop.cpp

HEADERS  += mainwindow.h \
    cvimage.h \
    firstwindow.h \
    thresholdop.h

FORMS    += mainwindow.ui \
    firstwindow.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    Media/logo.png


QMAKE_INFO_PLIST += \
 Info.plist
