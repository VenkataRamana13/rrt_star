#pragma once
#include <SFML/Graphics.hpp>
#include "../grid/grid.h"

class Visualizer {
public:
    Visualizer(int cols, int rows, bool randomize);
    void run();

private:
    sf::RenderWindow window;
    Grid grid;

    void handleEvents();
    void render();
};
