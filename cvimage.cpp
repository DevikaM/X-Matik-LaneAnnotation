#include "cvimage.h"

QSize CVImage::sizeHint() const
{
    return _qimage.size();
}

QSize CVImage::minimumSizeHint() const
{
    return _qimage.size();
}

void CVImage::showImage(const cv::Mat& image)
{
    _tmpRaw = image;
    switch (image.type()) {
           case CV_8UC1:
               cvtColor(image, _tmp, CV_GRAY2RGB);
               break;
           case CV_8UC3:
               cvtColor(image, _tmp, CV_BGR2RGB);
               break;
    }

    cvtColor(_tmp,_tmpGray, CV_RGB2GRAY);
    cvtColor(_tmpGray, _tmp, CV_GRAY2RGB);


    assert(_tmp.isContinuous());
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    this->setFixedSize(image.cols, image.rows);
    repaint();
    _imgVec->push_back(_qimage);

}

void CVImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}

void CVImage::scaleImage(QSize size)
{
    _qimage = _qimage.scaled(size, Qt::IgnoreAspectRatio,Qt::FastTransformation);
    //this->setFixedSize(size.height(), size.width());
    //repaint();
}

void CVImage::applyThreshold(int thresh)
{
    cv::threshold(_tmp,_tmpGray, thresh, 255, 0);
    _qimage = QImage(_tmpGray.data, _tmpGray.cols, _tmpGray.rows, _tmpGray.cols*3, QImage::Format_RGB888);
    repaint();
    _lastThresh = thresh;
    _imgVec->push_back(_qimage);
}

void CVImage::mousePressEvent(QMouseEvent *event)
{
    _drawing = true;
    _prevPoint = event->pos();
    QRectF rect(event->pos().x() - (_penWidth/2 ), event->pos().y() - (_penWidth/2), _penWidth,_penWidth);
    drawCircle(rect, true);
}


void CVImage::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && _drawing)
        drawTo(event->pos());
    else if(event->pos().x() <= 20 ||
            event->pos().y() <= 3 ||
            event->pos().x() >= this->width()-1 ||
            event->pos().y() >= this->height()-1)
        _qimage = _imgVec->back();
    else if(!_drawing)
    {
        _qimage = _imgVec->back();
        QRectF rect(event->pos().x() - (_penWidth/2 ), event->pos().y() - (_penWidth/2), _penWidth,_penWidth);
        drawCursor(rect);

    }


}

void CVImage::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && _drawing){
        drawTo(event->pos());
        _drawing = false;
        _imgVec->push_back(_qimage);
    }

}
void CVImage::drawTo(const QPoint &point)
{
     QPainter painter(&_qimage);
     painter.setPen(QPen(_color, _penWidth, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
     painter.drawLine(_prevPoint, point);
     repaint();
    _prevPoint = point;
}

void CVImage::drawCursor(QRectF rect){

    QPainter painter(&_qimage);
    painter.setPen(QPen(_penColor, 2, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    painter.drawEllipse(rect);
    repaint();

}
void CVImage::drawCircle(QRectF rect, bool onClick, bool cancelMove)
{
    if(cancelMove)
        this->setMouseTracking(false);
    if(!onClick){
        _imgVec->push_back(_qimage);
        _qimage = _imgVec->front();
        this->_color = Qt::black;
    }
    QPainter painter(&_qimage);
    painter.setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    if(onClick) painter.setBrush(_color);
    painter.drawEllipse(rect);
    repaint();

}

void CVImage::setColor(QColor color)
{
    _color = color;
}
void CVImage::setPenColor(QColor color)
{
    _penColor = color;
}
void CVImage::setWidth(int width)
{
    _penWidth = width;
}

void CVImage::reset()
{
     _imgVec->clear();
     _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
     _imgVec->push_back(_qimage);
     repaint();
}

bool CVImage::undo()
{
    if(_imgVec->size() == 1)
       _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    else{
        _qimage = _imgVec->back();
        _imgVec->pop_back();
    }

    repaint();
    return true;
}

void CVImage::save(std::string filePath, bool raw)
{
    if (raw){
        cv::imwrite(filePath,_tmpRaw);
    }else{
        QString path = QString::fromStdString(filePath);
        _imgVec->back().save(path);
        //_qimage.save(path);
    }
}

void CVImage::saveNeg(std::string filePath)
{
    cv::Mat temp(_tmp.rows,_tmp.cols, CV_8UC3, cv::Scalar(0,0,0));
    cv::imwrite(filePath,temp);
}
