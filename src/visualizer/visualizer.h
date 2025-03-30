#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <atomic> 
#include "../grid/grid.h"

class Visualizer {
public:
    Visualizer(int cols, int rows, bool randomize, bool addStub, bool addrrt);
	 void run(std::atomic<bool>& triggerRRT, std::atomic<bool>& running);

private:
    sf::RenderWindow window;
    Grid grid;
	 int cols; 
	 int rows; 

    void handleEvents();
    void render();
	 void generateStubPath();
	 void runRRT(); 
	 void generateRRTPath(); 
};
