#ifndef _QUADTREE_HPP_
#define _QUADTREE_HPP_
#include "quadnode.hpp"

class QuadTree {
private:
    int m;
    void split(QuadNode* node);

public:
    QuadNode* root;
    QuadTree(int m, double x0, double y0, double x1, double y1);
    bool insert(double x, double y);
    bool insertRecursive(QuadNode* node, Point* p);
    void destroyNode(QuadNode* node);
    ~QuadTree();
    bool find(QuadNode* node, Point* p);
    double distance(const Point* p1, const Point* p2);
    bool search(QuadNode* node, const Point* p, double d);
    Point* nearestPoint(QuadNode* node, const Point* p, Point*& nearest, double& nearestDist);
    int countPoints(QuadNode* node);
    void rangeSearch(QuadNode* node, double xr0, double yr0, double xr1, double yr1, bool& found);
    bool bboxCloser(QuadNode* node, const Point* p, double nearestDist);


};
#endif