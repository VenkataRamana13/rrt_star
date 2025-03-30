# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# SFML flags (adjust if installed via vcpkg, brew, etc.)
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Files
SRCS = src/main.cpp src/grid/grid.cpp src/visualizer/visualizer.cpp src/rrt/rrt.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = rrt_visualizer

# Build rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(SFML_LIBS)

clean:
	rm -f $(TARGET) *.o

