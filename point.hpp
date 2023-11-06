#ifndef _POINT_HPP_
#define _POINT_HPP_

class Point {
public:
    double x, y;
    Point(double x, double y);
    friend bool operator==(const Point& lhs, const Point& rhs);
};

#endif