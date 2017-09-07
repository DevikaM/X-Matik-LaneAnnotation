#include "square.h"

using namespace std;

Square::Square(int fx, int fy)
{
    first_x = fx;
    first_y = fy;
    second_x = 0;
    second_y = 0;
}

Square::Square(QPoint point)
{
    _first = point;
}

string Square::print()
{
    return to_string(_first.x()) + " " + to_string(_first.y()) + " " + to_string(getWidth()) + " " + to_string(getHeight());

}
/*fstream& operator<<(fstream& out, const Square& sqr){
    string str = to_string(sqr._first.x()) + " " + to_string(sqr._first.y()) + " " + to_string(sqr._second.x()) + " " + to_string(sqr._second.y());
    out << str;
    return out;
}*/
string Square::toString()
{
    if (!(first_x < second_x)){
        int swap_x=second_x, swap_y=second_y;
        second_x = first_x;
        second_y = first_y;
        first_x = swap_x;
        first_y = swap_y;
    }
    string output = string (to_string(first_x) +" "+ to_string(first_y)+" " + to_string(getWidth()) +" "+ to_string(getHeight()));
    return output;
}

void Square::setLast(int lx, int ly)
{
    second_x = lx;
    second_y = ly;
}

void Square::setLast(QPoint point)
{
    _second = point;
}

int Square::getFX()
{
    return first_x;
}

int Square::getFY()
{
    return first_y;
}

int Square::getHeight(){
    //return second_y - first_y;
    return _second.y() - _first.y();
}

int Square::getWidth(){
    return _second.x() - _first.x();
}
