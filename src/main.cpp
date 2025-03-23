// SFML + Grid + Visualizer Scaffold
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int cellSize = 20;
const int cols = 40;
const int rows = 30;

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
    Grid(int cols, int rows) : cols(cols), rows(rows) {
        grid.resize(rows, std::vector<Cell>(cols));
    }

    void draw(sf::RenderWindow& window) const {
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

    void setCell(int x, int y, CellType type) {
        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            grid[y][x].type = type;
        }
    }

    CellType getCell(int x, int y) const {
        if (x >= 0 && x < cols && y >= 0 && y < rows) {
            return grid[y][x].type;
        }
        return CellType::FREE;
    }

private:
    int cols, rows;
    std::vector<std::vector<Cell>> grid;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(cols * cellSize, rows * cellSize), "RRT* Visualizer");
    window.setFramerateLimit(60);

    Grid grid(cols, rows);
    grid.setCell(1, 1, CellType::START);
    grid.setCell(cols - 2, rows - 2, CellType::GOAL);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
				int x = static_cast<int>(worldPos.x) / cellSize;
				int y = static_cast<int>(worldPos.y) / cellSize;
            if (x >= 0 && x < cols && y >= 0 && y < rows) {
                grid.setCell(x, y, CellType::OBSTACLE);
            }
        }

        window.clear(sf::Color::White);
        grid.draw(window);
        window.display();
    }

    return 0;
}

