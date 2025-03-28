#include "grid.h"

const int cellSize = 20;

Grid::Grid(int cols, int rows) : cols(cols), rows(rows) {
    grid.resize(rows, std::vector<Cell>(cols));
}

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

void Grid::setCell(int x, int y, CellType type) {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        grid[y][x].type = type;
    }
}

CellType Grid::getCell(int x, int y) const {
    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        return grid[y][x].type;
    }
    return CellType::FREE;
}

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

