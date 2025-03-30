#include "grid.h"

const int cellSize = 20;

//set the size of the grid
Grid::Grid(int cols, int rows) : cols(cols), rows(rows) {
    grid.resize(rows, std::vector<Cell>(cols));
}

//loops through entire cell in the grid and assigns color based on its cell type
void Grid::draw(sf::RenderWindow& window) const {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::RectangleShape rect(sf::Vector2f(cellSize - 1, cellSize - 1));
            rect.setPosition(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize));

            switch (grid[y][x].type) {
                case CellType::FREE: rect.setFillColor(sf::Color::White); break;
                case CellType::OBSTACLE: rect.setFillColor(sf::Color::Black); break;
                case CellType::START: rect.setFillColor(sf::Color::Green); break;
                case CellType::GOAL: rect.setFillColor(sf::Color::Red); break;
                case CellType::PATH: rect.setFillColor(sf::Color::Blue); break;
                case CellType::TREE: rect.setFillColor(sf::Color(150, 150, 150)); break;
            }

            window.draw(rect);
        }
    }
}

//sets cell according to its type
void Grid::setCell(int x, int y, CellType type) {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        grid[y][x].type = type;
    }
}

//gets cell type: primarily for checking if the cell is an obstacle
CellType Grid::getCell(int x, int y) const {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        return grid[y][x].type;
    }
    return CellType::FREE;
}

//function to generate random obstacles
void Grid::generateRandomObstacles() {
	 //fill quarter of the space with obstacles
	 float obstacleProbability = 0.25; 
	 
	 //random number generation between 0 and 1 - dis(gen)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

	 //iterate through all the blocks and if dis(gen) is less than 0.25
	 //assign it as obstacle
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
				//continue if start node or end node
				if(x == 1 and y == 1) continue; 
				if(x == rows - 2 and y == cols - 2) continue; 
						  
            if (dis(gen) < obstacleProbability) {
                grid[x][y].type = CellType::OBSTACLE;
            }
        }
    }
}

//for each of the points in rrt path: assigns path
void Grid::addRRTPath(const std::vector<std::pair<int, int>>& path) {
    for (const auto& [x, y] : path) {
        // Avoid overwriting start and goal
        if (grid[y][x].type != CellType::START && grid[y][x].type != CellType::GOAL) {
            grid[y][x].type = CellType::PATH;
        }
    }
}

//ignore:for testing
void Grid::addStubPath(const std::vector<std::pair<int, int>>& path) {
    for (auto [x, y] : path) {
        if (getCell(x, y) == CellType::FREE) {
            setCell(x, y, CellType::PATH);
        }
    }
}

//to check if line is free between points(x1, y1) and (x2, y2)
//uses bresenham's line drawing algorithm 
bool Grid::lineIsFree(int x1, int y1, int x2, int y2) const {
    int dx = std::abs(x2 - x1);
    int dy = -std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (getCell(x1, y1) == CellType::OBSTACLE)
            return false;

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }

    return true;
}
