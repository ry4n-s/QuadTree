#ifndef _QUADNODE_HPP_
#define _QUADNODE_HPP_
#include "point.hpp"

class QuadNode {
public:
    double x0, y0, x1, y1;
    Point** points;
    int count;
    QuadNode *topLeft, *topRight, *bottomLeft, *bottomRight;
    int capacity;
    bool isLeaf;  

    QuadNode(double x0, double y0, double x1, double y1, int capacity);
    ~QuadNode();

    bool contains(const Point* p);
    bool pointExists(Point* p);
};

#endif
