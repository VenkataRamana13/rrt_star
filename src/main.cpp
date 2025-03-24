#include <iostream>
#include "visualizer/visualizer.h"

int main() {
    const int cols = 40;
    const int rows = 30;

    char mode;
    std::cout << "Run in random mode (r) or interactive mode (i)? ";
    std::cin >> mode;

    bool randomize = (mode == 'r' || mode == 'R');
    Visualizer visualizer(cols, rows, randomize);
    visualizer.run();

    return 0;
}
