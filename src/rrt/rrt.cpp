#include "rrt.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp> 

RRT::RRT(int cols_, int rows_, Grid& grid_, sf::RenderWindow& window_, int maxIter, int stepSz, double radius_)
    : cols(cols_), rows(rows_), grid(grid_), window(window_), maxIterations(maxIter), stepSize(stepSz), radius(radius_) {
//    auto [sx, sy] = 1, 1; 
//    auto [gx, gy] = 38, 28; 

	 int sx = 1, sy = 1; 
	 int gx = cols - 2, gy = rows - 2; 

    start = new Node(sx, sy);
    goal = new Node(gx, gy);
    nodes.push_back(start);
}

bool RRT::run() {
    for (int i = 0; i < maxIterations; ++i) {
        Node* xRand = sample();
        Node* xNear = nearest(xRand);
        Node* xNew = steer(xNear, xRand);

        if (!xNew || !isCollisionFree(xNear->x, xNear->y, xNew->x, xNew->y)) {
            delete xNew;
            delete xRand;
            continue;
        }

        auto neighbors = near(xNew);

        Node* bestParent = xNear;
        double minCost = xNear->cost + distance(xNear, xNew);

        for (Node* n : neighbors) {
            if (isCollisionFree(n->x, n->y, xNew->x, xNew->y)) {
                double c = n->cost + distance(n, xNew);
                if (c < minCost) {
                    bestParent = n;
                    minCost = c;
                }
            }
        }

        xNew->parent = bestParent;
        xNew->cost = minCost;
        nodes.push_back(xNew);
        grid.setCell(xNew->x, xNew->y, CellType::TREE);
		  window.clear();
		  grid.draw(window);
		  window.display();
		  std::this_thread::sleep_for(std::chrono::milliseconds(200));

        rewire(xNew, neighbors);

        if (isGoalReached(xNew)) {
            goal->parent = xNew;
            return true;
        }

        delete xRand;
    }
    return false;
}

std::vector<std::pair<int, int>> RRT::getPath() const {
    std::vector<std::pair<int, int>> path;
    Node* current = goal;
    while (current) {
        path.emplace_back(current->x, current->y);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

Node* RRT::sample() {
    int x = rand() % cols; 
    int y = rand() % rows; 
    return new Node(x, y);
}

Node* RRT::nearest(Node* node) {
    Node* best = nullptr;
    double bestDist = 1e9;
    for (Node* n : nodes) {
        double d = distance(n, node);
        if (d < bestDist) {
            bestDist = d;
            best = n;
        }
    }
    return best;
}

Node* RRT::steer(Node* from, Node* to) {
    int dx = to->x - from->x;
    int dy = to->y - from->y;

    // Move one cell in one of the 4 directions (N, S, E, W)
    if (std::abs(dx) > std::abs(dy)) {
        dx = (dx > 0) ? 1 : -1;
        dy = 0;
    } else {
        dy = (dy > 0) ? 1 : -1;
        dx = 0;
    }

    int newX = from->x + dx;
    int newY = from->y + dy;

	 if(newX > cols - 1 or newY > rows - 1 or newX < 0 or newY < 0) return nullptr; 
    return new Node(newX, newY);
}

std::vector<Node*> RRT::near(Node* node) {
    std::vector<Node*> neighbors;
    for (Node* n : nodes) {
        if (distance(n, node) < radius)
            neighbors.push_back(n);
    }
    return neighbors;
}

void RRT::rewire(Node* xNew, const std::vector<Node*>& neighbors) {
    for (Node* n : neighbors) {
        double newCost = xNew->cost + distance(xNew, n);
        if (newCost < n->cost && isCollisionFree(xNew->x, xNew->y, n->x, n->y)) {
            n->parent = xNew;
            n->cost = newCost;
        }
    }
}

bool RRT::isCollisionFree(int x1, int y1, int x2, int y2) {
    // use Grid's internal function
    return grid.lineIsFree(x1, y1, x2, y2);
}

int RRT::distance(Node* a, Node* b) {
	 return std::abs(a->x - b->x) + std::abs(a->y - b->y); 
}

bool RRT::isGoalReached(Node* node) {
    return distance(node, goal) <= stepSize;
}
