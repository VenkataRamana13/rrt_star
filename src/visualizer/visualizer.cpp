#include "visualizer.h"
#include "../rrt/rrt.h"
 
float obstacleProbability = 0.25; 

void Visualizer::run(std::atomic<bool>& triggerRRT, std::atomic<bool>& running) {
    while (window.isOpen() && running) {
        handleEvents();
        render();

        if (triggerRRT) {
            runRRT();           // existing function
            triggerRRT = false; // reset trigger
        }
    }
}

std::vector<std::pair<int, int>> generateStubPath(int startX, int startY, int goalX, int goalY) {
    std::vector<std::pair<int, int>> path;
    int midX = (startX + goalX) / 2;

    for (int x = startX; x <= midX; ++x)
        path.emplace_back(x, startY);

    for (int y = startY; y <= goalY; ++y)
        path.emplace_back(midX, y);

    for (int x = midX + 1; x <= goalX; ++x)
        path.emplace_back(x, goalY);

    return path;
}

Visualizer::Visualizer(int cols, int rows, bool randomize, bool addStub, bool addrrt)
    : window(sf::VideoMode(cols * cellSize, rows * cellSize), "RRT* Visualizer"),
      grid(cols, rows), cols(cols), rows(rows) {
    window.setFramerateLimit(60);
    grid.setCell(1, 1, CellType::START);
    grid.setCell(cols - 2, rows - 2, CellType::GOAL);

    if (randomize) {
        grid.generateRandomObstacles();
    }
	 
	 if(addStub){
		  generateStubPath(); 
	 }
	 
	 if(addrrt){
		  runRRT(); 
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

void Visualizer::generateStubPath() {
    auto path = ::generateStubPath(1, 1, (window.getSize().x / cellSize) - 2, (window.getSize().y / cellSize) - 2);
    grid.addStubPath(path);
}

void Visualizer::runRRT(){
	 std::cout << "Running RRT*..." << std::endl;

    // Parameters: iterations, step size, radius
    int maxIterations = 2000;
    int stepSize = 1;
    double radius = 10.0;

    RRT rrt(cols, rows, grid, window, maxIterations, stepSize, radius);
    bool success = rrt.run();

    if (success) {
        std::cout << "Path found.\n";
        auto path = rrt.getPath();
        grid.addRRTPath(path);
    } else {
        std::cout << "Failed to find a path.\n";
    }
}
