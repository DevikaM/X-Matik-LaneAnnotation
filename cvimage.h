#pragma once
#include "opencv2/opencv.hpp"

#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QtGui>
#include <QMainWindow>
#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QList>
#include <QBrush>
#include "thresholdop.h"
#include "square.h"
#include <vector>
#include <iostream>

class CVImage : public QWidget
{
    Q_OBJECT
public:
    explicit CVImage(QWidget *parent = 0): QWidget(parent){
            _penColor = Qt::white;
            _color = Qt::blue;
            _drawing = false;
            _penWidth =25;
            _lastThresh = 0;
            _imgVec= new std::vector<QImage>;
            _squares = new std::vector<Square>;
            setMouseTracking(true);
    }

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    int getSquares();
    cv::Mat _tmpRaw;
    std::vector<Square>* _squares;


public slots:
    void showImage(const cv::Mat& image);
    void scaleImage(QSize size);
    void applyThreshold(int thresh);
    void setPenColor(QColor c);
    void setColor(QColor c);
    void setWidth(int width);
    bool undo();
    void saveBox(std::string filePath);
    void save(std::string filePath, bool raw = false);
    void saveNeg(std::string filePath);
    void drawCursor(QRectF rect);
    void displayCursor(QRectF rect, bool onClick = false, bool cancelMove = false);
    void drawCircle(QRectF rect, bool onClick = false, bool cancelMove = false);
    void drawCross(QRectF rect, bool onClick = false, bool cancelMove = false);
    void reset();
    void configure(int shape, int cursor);


protected:
   void paintEvent(QPaintEvent* e);
   void mousePressEvent(QMouseEvent *event) override;
   void mouseMoveEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;
   void drawTo(const QPoint &point);
   void drawSquare(const QPoint &point);

   std::vector<QImage> *_imgVec;
   QImage _qimage;
   cv::Mat _tmp;
   cv::Mat _tmpGray;
   QPoint _prevPoint;
   bool _drawing;
   int _penWidth;
   int _lastThresh;
   QColor _color;
   QColor _penColor;
   int _cursor;
   int _shape;


};

