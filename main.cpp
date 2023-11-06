#include <iostream>
#include "quadtree.hpp"
#include "illegal_exception.hpp"
#include <string>
#include <cmath>

int main() {
    std::string command;
    QuadTree* tree = nullptr;

    while (true) {
        std::cin >> command;

        if (command == "EXIT") break;

        if (command == "INIT") {
            double m, x0, y0, x1, y1;

            if (!(std::cin >> m >> x0 >> y0 >> x1 >> y1)) {
                if (std::cin.peek() == '\n') std::cin.ignore(); 
                std::cout << "illegal argument" << std::endl;
                std::cin.clear();
                while (std::cin.get() != '\n');
                continue;
            }

            try {
                if (m > 4 && x0 < x1 && y0 < y1) {
                    tree = new QuadTree(m, x0, y0, x1, y1);
                    std::cout << "success" << std::endl;
                } else {
                    throw illegal_exception();
                }
            } catch (const illegal_exception& e) {
                std::cout << e.what() << std::endl;
                std::cin.clear();
                char ch;
                while (std::cin.get(ch) && ch != '\n');
            }
        }
        else if (command == "INSERT") {
            double x, y;

            if (!(std::cin >> x >> y)) {
                if (std::cin.peek() == '\n') std::cin.ignore();  
                std::cout << "illegal argument" << std::endl;
                std::cin.clear();
                while (std::cin.get() != '\n');
                continue;
            }

            if (tree && tree->insert(x, y)) {
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }
        }
        else if (command == "SEARCH") {
            double x, y, d;

            if (!(std::cin >> x >> y >> d)) {
                if (std::cin.peek() == '\n') std::cin.ignore();  
                std::cout << "illegal argument" << std::endl;
                std::cin.clear();
                while (std::cin.get() != '\n');
                continue;
            }

            Point p(x, y);
            if (tree && tree->search(tree->root, &p, d)) {
                std::cout << "point exists" << std::endl;
            } else {
                std::cout << "no point exists" << std::endl;
            }
        }

        else if (command == "NEAREST") {
            double x, y;

            if (!(std::cin >> x >> y)) {
                if (std::cin.peek() == '\n') std::cin.ignore(); 
                std::cout << "illegal argument" << std::endl;
                std::cin.clear();
                while (std::cin.get() != '\n');
                continue;
            }

            Point queryPoint(x, y);
            Point* nearest = nullptr;
            double nearestDist = 2e31-1;  //start with a very large distance

            tree->nearestPoint(tree->root, &queryPoint, nearest, nearestDist);

            if (nearest) {
                std::cout << nearest->x << " " << nearest->y << std::endl;
            } else {
                std::cout << "no point exists" << std::endl;
            }
        }

        else if (command == "RANGE") {
            double xr0, yr0, xr1, yr1;

            if (!(std::cin >> xr0 >> yr0 >> xr1 >> yr1)) {
                if (std::cin.peek() == '\n') std::cin.ignore(); 
                std::cout << "illegal argument" << std::endl;
                std::cin.clear();
                while (std::cin.get() != '\n');
                continue;
            }

            if (xr0 >= xr1 || yr0 >= yr1) {
                std::cout << "illegal argument" << std::endl;
                continue;  //go to the next iteration of the loop
            }

            bool foundPoints = false;
            tree->rangeSearch(tree->root, xr0, yr0, xr1, yr1, foundPoints);

            if (!foundPoints) {
                std::cout << "no points within range" << std::endl;
            } else {
                std::cout << std::endl;
            }
        }

        else if (command == "NUM") {
            int totalPoints = tree->countPoints(tree->root);
            std::cout << totalPoints << std::endl;
        }

        else {
            //invalid command
            std::cout << "illegal argument" << std::endl;
        }
    }

    delete tree;
    return 0;
}
        