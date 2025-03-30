#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random> 
#include <thread>
#include <chrono> 

extern const int cellSize;

enum class CellType {
    FREE,
    OBSTACLE,
    START,
    GOAL,
    PATH,
    TREE
};

struct Cell {
    CellType type = CellType::FREE;
};

class Grid {
public:
    Grid(int cols, int rows);

    void draw(sf::RenderWindow& window) const;
    void setCell(int x, int y, CellType type);
    CellType getCell(int x, int y) const;
	 void generateRandomObstacles(); 
	 void addStubPath(const std::vector<std::pair<int, int>>& path);
	 void addRRTPath(const std::vector<std::pair<int, int>>& path); 
	 bool lineIsFree(int, int, int, int) const; 

private: int cols, rows;
    std::vector<std::vector<Cell>> grid;
};

