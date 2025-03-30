#include <iostream>
#include <thread>
#include <atomic>
#include "visualizer/visualizer.h"

// Shared flags
std::atomic<bool> triggerRRT(false);
std::atomic<bool> running(true);

//take contionuous input from user even after pop up window
void inputThreadLoop() {
    std::string command;
    while (running) {
        std::cout << "Enter command (rrt / quit): ";
        std::cin >> command;

        if (command == "rrt") {
            triggerRRT = true;
        } else if (command == "quit") {
            running = false;
        }
    }
}

int main() {
	 //cols and rows values
    //const int cols = 40;
    //const int rows = 30;

    bool randomize = false;
    bool addStub = false;
    bool addrrt = false;
	 
	 int cols, rows; 
	 std::cout << "Input number of cols: "; 
	 std::cin >> cols; 
	 std::cout << "Input number of rows: "; 
	 std::cin >> rows; 
	 
	 std::pair<int, int> start = {1, 1}, end = {cols - 1, rows - 1}; 
	 bool start_ = false; char s;  
	 std::cout << "Input start coords?(y/n): "; 
	 std::cin >> s; if(s == 'y' or s == 'Y') start_ = true;  
	 if(start_ == true){
		  std::cout << "Input start coords: (example: 1 1): "; 
		  std::cin >> start.first >> start.second; 
	 }

	 bool end_ = false; 
	 std::cout << "Input end coords?(y/n): "; char e;   
	 std::cin >> e; if(e == 'y' or e == 'Y') end_ = true; 
	 if(end_ == true){
		  std::cout << "Input end coords: (example cols-1 rows-1): "; 
		  std::cin >> end.first >> end.second; 
	 }
	
	 //randomizing obstacles
	 char r;
	 std::cout << "randomize obstacles?(y/n)"; 
	 std::cin >> r; 
	 if(r == 'y' or r == 'Y') randomize = true; 

	 //addStub is inconsequential: added for conveniance during testing
    Visualizer visualizer(cols, rows, start, end, randomize, addStub, addrrt);

    // Start input listener thread
    std::thread inputThread(inputThreadLoop);

    // Start visualization (runs while 'running' is true)
    visualizer.run(triggerRRT, running);

    inputThread.join();  // Wait for input thread to finish
    return 0;
}
