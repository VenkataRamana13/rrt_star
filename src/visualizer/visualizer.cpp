#include "visualizer.h"
 
float obstacleProbability = 0.25; 

Visualizer::Visualizer(int cols, int rows, bool randomize)
    : window(sf::VideoMode(cols * cellSize, rows * cellSize), "RRT* Visualizer"),
      grid(cols, rows) {
    window.setFramerateLimit(60);
    grid.setCell(1, 1, CellType::START);
    grid.setCell(cols - 2, rows - 2, CellType::GOAL);

    if (randomize) {
        grid.generateRandomObstacles();
    }
}

void Visualizer::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void Visualizer::handleEvents() {
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
        grid.setCell(x, y, CellType::OBSTACLE);
    }
}

void Visualizer::render() {
    window.clear(sf::Color::White);
    grid.draw(window);
    window.display();
}

