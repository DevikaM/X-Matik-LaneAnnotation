#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <QtCore>
#include <string>
#include <stdio.h>
#include <fstream>


class Square
{

public:
    Square(int fx, int fy);
    Square(QPoint);
    std::string toString();
    std::string print();
    QPoint _first;
    QPoint _second;
    int first_x;
    int first_y;
    int second_x;
    int second_y;
    void setLast(int lx, int ly);
    void setLast(QPoint point);
    int getFX();
    int getFY();
    int getWidth();
    int getHeight();

};

//std::ostream& operator<<(std::ostream& out, const Square& sqr);

#endif // SQUARE_H
