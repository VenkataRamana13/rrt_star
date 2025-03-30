#include <iostream>
#include <thread>
#include <atomic>
#include "visualizer/visualizer.h"

// Shared flags
std::atomic<bool> triggerRRT(false);
std::atomic<bool> running(true);

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
    const int cols = 40;
    const int rows = 30;

    bool randomize = false;
    bool addStub = false;
    bool addrrt = false;

    Visualizer visualizer(cols, rows, randomize, addStub, addrrt);

    // Start input listener thread
    std::thread inputThread(inputThreadLoop);

    // Start visualization (runs while 'running' is true)
    visualizer.run(triggerRRT, running);

    inputThread.join();  // Wait for input thread to finish
    return 0;
}
