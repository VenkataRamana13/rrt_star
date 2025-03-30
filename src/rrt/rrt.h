#pragma once

#include "../grid/grid.h"
#include "node.h"
#include <vector>
#include <SFML/Graphics.hpp>

class RRT {
public:
    RRT(int cols, int rows, Grid& grid, sf::RenderWindow& window, int maxIterations, int stepSize, double radius);

    bool run();
    std::vector<std::pair<int, int>> getPath() const;

private:
	 int cols; 
	 int rows; 
    Grid& grid;
	 sf::RenderWindow& window; 
    int maxIterations;
    int stepSize;
    double radius;

    std::vector<Node*> nodes;
    Node* start;
    Node* goal;

    Node* sample();
    Node* nearest(Node* node);
    Node* steer(Node* from, Node* to);
    std::vector<Node*> near(Node* node);
    void rewire(Node* xNew, const std::vector<Node*>& neighbors);
    bool isCollisionFree(int x1, int y1, int x2, int y2);
    int distance(Node* a, Node* b);
    bool isGoalReached(Node* node);
};
