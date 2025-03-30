#pragma once

//node data structure which stores location, cost from start, and its parent node; 
struct Node {
    int x, y;
    double cost;
    Node* parent;

    Node(int x_, int y_) : x(x_), y(y_), cost(0), parent(nullptr) {}
};
