#include "visualizer.h"
#include "../rrt/rrt.h"
 
float obstacleProbability = 0.25; 

//wrapper visualizer 
void Visualizer::run(std::atomic<bool>& triggerRRT, std::atomic<bool>& running) {
    while (window.isOpen() && running) {
        handleEvents();
        render();

		  //triggerRRT is true if 'rrt' input in main
        if (triggerRRT) {
            runRRT();           // existing function
            triggerRRT = false; // reset trigger
        }
    }
}

//ignore: for testing
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

//sets cols and rows, if 'rrt' prompted from user, runs addrrt according to other parameters set in main
Visualizer::Visualizer(int cols, int rows, std::pair<int, int>& start, std::pair<int, int>& end, bool randomize, bool addStub, bool addrrt)
    : window(sf::VideoMode(cols * cellSize, rows * cellSize), "RRT* Visualizer"),

      grid(cols, rows), cols(cols), rows(rows), start(start), end(end) {
    window.setFramerateLimit(60);
	 view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)); 
	 window.setView(view); 
	 
	 //sets start and goal 
    grid.setCell(start.first, start.second, CellType::START);
    grid.setCell(end.first, end.second, CellType::GOAL);

    if (randomize) {
        grid.generateRandomObstacles(start, end);
    }
	 
	 if(addStub){
		  generateStubPath(); 
	 }
	 
	 if(addrrt){
		  runRRT(); 
	 }
}

//handles input from user
void Visualizer::handleEvents() {
    sf::Event event;
	 //if pressed on close, close window
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }

	 //sets obstacles with mouse's left click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        int x = static_cast<int>(worldPos.x) / cellSize;
        int y = static_cast<int>(worldPos.y) / cellSize;
        grid.setCell(x, y, CellType::OBSTACLE);
    }
	 
	 //for scrolling
	 float scrollSpeed = 50.0f; 
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		  view.move(-scrollSpeed, 0);
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		  view.move(scrollSpeed, 0);
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		  view.move(0, -scrollSpeed);
	 }
	 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		  view.move(0, scrollSpeed);
	 }
	 window.setView(view); 
}

void Visualizer::render() {
	 //background color white
    window.clear(sf::Color::White);
	 window.setView(view); 
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
    int maxIterations = 20000;
    int stepSize = 1;
    double radius = 10.0;

	 //checks if rrt path exists
    RRT rrt(cols, rows, start, end, grid, window, maxIterations, stepSize, radius);
    bool success = rrt.run();

	 //prints path found if rrt successful else prints failed as below 
    if (success) {
        std::cout << "Path found.\n";
        auto path = rrt.getPath();
        grid.addRRTPath(path);
    } else {
        std::cout << "Failed to find a path.\n";
    }
}
