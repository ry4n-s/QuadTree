#include <iostream>
#include <cmath>
#include "quadtree.hpp"

QuadTree::QuadTree(int m, double x0, double y0, double x1, double y1): m(m) {
    root = new QuadNode(x0, y0, x1, y1, m);
}

QuadTree::~QuadTree() {
    //recursive deletion of all nodes
    destroyNode(root);
}

bool QuadTree::insert(double x, double y) {
    Point* p = new Point(x, y);
    if (insertRecursive(root, p)) {
        return true; //successfully inserted
    } else {
        delete p; //clean up if not inserted
        return false; //insertion failed
    }
}

bool QuadTree::insertRecursive(QuadNode* node, Point* p) {

    //check if the node contains the point
    if (!node->contains(p)) {
        return false;
    }

    //immediately check if the point exists in the node
    if (node->pointExists(p)) {
        return false;
    }

if (!node->isLeaf) {
    //now try inserting into child nodes
    if (insertRecursive(node->topLeft, p)) return true;
    if (insertRecursive(node->topRight, p)) return true;
    if (insertRecursive(node->bottomLeft, p)) return true;
    if (insertRecursive(node->bottomRight, p)) return true;
    return false;
}


    //check if we can add the point to this node
    if (node->count < node->capacity) {
        node->points[node->count++] = p;
        return true;
    }
    
    //if the node is full, we need to split it
    split(node);

    //now try inserting into child nodes after a possible split
    if (insertRecursive(node->topLeft, p)) return true;
    if (insertRecursive(node->topRight, p)) return true;
    if (insertRecursive(node->bottomLeft, p)) return true;
    if (insertRecursive(node->bottomRight, p)) return true;

    return false;
}


void QuadTree::split(QuadNode* node) {
    double midX = (node->x0 + node->x1) / 2;
    double midY = (node->y0 + node->y1) / 2;

    //create children
    node->topLeft = new QuadNode(node->x0, midY, midX, node->y1, m);
    node->topRight = new QuadNode(midX, midY, node->x1, node->y1, m);
    node->bottomLeft = new QuadNode(node->x0, node->y0, midX, midY, m);
    node->bottomRight = new QuadNode(midX, node->y0, node->x1, midY, m);

    //redistribute the points from the current node to the new children
    for (int i = 0; i < node->count; i++) {
        Point* currentPoint = node->points[i];
        
        if (node->topLeft->contains(currentPoint)) {
            node->topLeft->points[node->topLeft->count++] = currentPoint;
        } else if (node->topRight->contains(currentPoint)) {
            node->topRight->points[node->topRight->count++] = currentPoint;
        } else if (node->bottomLeft->contains(currentPoint)) {
            node->bottomLeft->points[node->bottomLeft->count++] = currentPoint;
        } else if (node->bottomRight->contains(currentPoint)) {
            node->bottomRight->points[node->bottomRight->count++] = currentPoint;
        }
    }

    delete[] node->points;  //delete the original array of points
    node->count = 0;
    node->points = nullptr;  //set the node's points pointer to nullptr for safety
    node->isLeaf = false;
}

void QuadTree::destroyNode(QuadNode* node) {
    if (!node) return; 

    //delete all points inside the node
    for (int i = 0; i < node->count; i++) {
        delete node->points[i];
        node->points[i] = nullptr;  
    }
    delete[] node->points;
    node->points = nullptr;  //set points array to nullptr after deletion

    //recursively delete child nodes
    destroyNode(node->topLeft);
    destroyNode(node->topRight);
    destroyNode(node->bottomLeft);
    destroyNode(node->bottomRight);

    delete node;
}


bool QuadTree::find(QuadNode* node, Point* p) {
    if (!node->contains(p)) return false;

    for (int i = 0; i < node->count; i++) {
        if (*node->points[i] == *p) return true;
    }

    if (!node->topLeft) return false;

    return find(node->topLeft, p) || 
           find(node->topRight, p) || 
           find(node->bottomLeft, p) || 
           find(node->bottomRight, p);
}

bool QuadTree::search(QuadNode* node, const Point* p, double d) {
    if (!node) return false;

    //check the distance of every point in this node to the given point
    for (int i = 0; i < node->count; i++) {
        if (distance(p, node->points[i]) < d) return true;
    }

    //if this node has been split, recursively search its children
    if (node->topLeft && node->topLeft->contains(p) && search(node->topLeft, p, d)) return true;
    if (node->topRight && node->topRight->contains(p) && search(node->topRight, p, d)) return true;
    if (node->bottomLeft && node->bottomLeft->contains(p) && search(node->bottomLeft, p, d)) return true;
    if (node->bottomRight && node->bottomRight->contains(p) && search(node->bottomRight, p, d)) return true;

    return false;
}

double QuadTree::distance(const Point* p1, const Point* p2) {
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

Point* QuadTree::nearestPoint(QuadNode* node, const Point* p, Point*& nearest, double& nearestDist) {
    if (!node) return nullptr;

    //check the distance of every point in this node to the given point
    for (int i = 0; i < node->count; i++) {
        double dist = distance(p, node->points[i]);
        if (!nearest || dist < nearestDist) {
            nearestDist = dist;
            nearest = node->points[i];
        }
    }

    //if this node has been split, recursively search its children
    //but only if their bounding boxes are closer than the nearestDist
    if (node->topLeft && bboxCloser(node->topLeft, p, nearestDist))
        nearestPoint(node->topLeft, p, nearest, nearestDist);
    if (node->topRight && bboxCloser(node->topRight, p, nearestDist))
        nearestPoint(node->topRight, p, nearest, nearestDist);
    if (node->bottomLeft && bboxCloser(node->bottomLeft, p, nearestDist))
        nearestPoint(node->bottomLeft, p, nearest, nearestDist);
    if (node->bottomRight && bboxCloser(node->bottomRight, p, nearestDist))
        nearestPoint(node->bottomRight, p, nearest, nearestDist);

    return nearest;
}

bool QuadTree::bboxCloser(QuadNode* node, const Point* p, double nearestDist) {
    //calculate the closest point in the node's bounding box to point p
    double closestX = std::max(node->x0, std::min(p->x, node->x1));
    double closestY = std::max(node->y0, std::min(p->y, node->y1));
    //calculate the distance from p to this closest point
    double dist = sqrt(pow(closestX - p->x, 2) + pow(closestY - p->y, 2));
    //check if this distance is less than the current nearest distance
    return dist < nearestDist;
}


int QuadTree::countPoints(QuadNode* node) {
    if (!node) return 0;

    int total = node->count;
    total += countPoints(node->topLeft);
    total += countPoints(node->topRight);
    total += countPoints(node->bottomLeft);
    total += countPoints(node->bottomRight);

    return total;
}

void QuadTree::rangeSearch(QuadNode* node, double xr0, double yr0, double xr1, double yr1, bool& found) {
    if (!node) return;

    //if the node's boundary does not intersect with the range, return
    if (node->x1 <= xr0 || node->x0 >= xr1 || node->y1 <= yr0 || node->y0 >= yr1) {
        return;
    }

    //check each point in this node if it lies within the range
    for (int i = 0; i < node->count; i++) {
        double x = node->points[i]->x;
        double y = node->points[i]->y;

        if (x > xr0 && x < xr1 && y > yr0 && y < yr1) {
            std::cout << x << " " << y << " ";
            found = true;
        }
    }

    //recurse into child nodes if the node has been split
    rangeSearch(node->topLeft, xr0, yr0, xr1, yr1, found);
    rangeSearch(node->topRight, xr0, yr0, xr1, yr1, found);
    rangeSearch(node->bottomLeft, xr0, yr0, xr1, yr1, found);
    rangeSearch(node->bottomRight, xr0, yr0, xr1, yr1, found);
}
