#pragma once

struct Node {
    int x, y;
    double cost;
    Node* parent;

    Node(int x_, int y_) : x(x_), y(y_), cost(0), parent(nullptr) {}
};
