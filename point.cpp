#include "point.hpp"

Point::Point(double x, double y) : x(x), y(y) {}

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
