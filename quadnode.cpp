#include <iostream>
#include "quadnode.hpp"

QuadNode::QuadNode(double x0, double y0, double x1, double y1, int capacity) : 
    x0(x0), y0(y0), x1(x1), y1(y1), 
    topLeft(nullptr), topRight(nullptr), 
    bottomLeft(nullptr), bottomRight(nullptr), 
    capacity(capacity), 
    isLeaf(true) {  //initialize the node as a leaf node
    points = new Point*[capacity];
    count = 0;
}

QuadNode::~QuadNode() {
    delete[] points;
}

bool QuadNode::contains(const Point* p) {
    return (p->x >= x0 && p->x < x1) &&
           (p->y >= y0 && p->y < y1);
}


bool QuadNode::pointExists(Point* p) {
    for (int i = 0; i < count; i++) {
        if (points[i]->x == p->x && points[i]->y == p->y) {
            return true;
        }
    }
    return false;
}
