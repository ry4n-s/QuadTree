# QuadTree 🌐🌳

## 🚀 Project Overview
Welcome to the QuadTree project! This repository hosts a sophisticated C++ implementation of a QuadTree, a tree data structure used for partitioning a two-dimensional space. It is particularly useful in applications like geographic information systems, collision detection in two-dimensional games, and area searches.

## 📌 Key Features
- **Robust QuadTree Implementation**: The project includes classes for QuadTree and its nodes, providing efficient spatial query capabilities.
- **Spatial Operations**: Features commands like `INSERT`, `SEARCH`, `NEAREST`, `RANGE`, and `NUM` to interact with the QuadTree, enabling insertion, search, nearest neighbor, range search, and counting points.
- **Error Handling**: Includes an `illegal_exception` class for handling invalid operations or arguments.
- **Interactive Command Interface**: Users can interact with the QuadTree through a console interface, entering commands and receiving feedback in real-time.

## 🎯 Application Scenarios
- **Geospatial Data Management**: Ideal for managing large sets of two-dimensional spatial data.
- **Computer Graphics and Game Development**: Useful for collision detection, rendering optimization, and spatial querying.
- **Research and Educational Purposes**: A great resource for understanding and experimenting with spatial data structures.

## 🔍 How It Works
- **QuadNode**: Represents each quadrant in the tree, holding points and references to its child nodes.
- **Spatial Queries**: Insert points, search for points within a certain distance, find the nearest point, and search within a specific range.
- **Dynamic Resizing**: Splits nodes dynamically based on the capacity and the distribution of points.
