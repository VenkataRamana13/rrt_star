#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <atomic> 
#include "../grid/grid.h"
#include <utility>

class Visualizer {
public:
    Visualizer(int cols, int rows, std::pair<int, int>& start, std::pair<int, int>& end, bool randomize, bool addStub, bool addrrt);
	 void run(std::atomic<bool>& triggerRRT, std::atomic<bool>& running);

private:
    sf::RenderWindow window;
	 sf::View view; 
    Grid grid;
	 int cols; 
	 int rows; 
	 std::pair<int, int>& start, end; 

    void handleEvents();
    void render();
	 void generateStubPath();
	 void runRRT(); 
	 void generateRRTPath(); 
};
