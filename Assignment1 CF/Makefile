CXX := g++
CXXFLAGS := -std=c++17 -O2

BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /usr/local)

INCLUDES := -I$(BREW_PREFIX)/include
LIBS := -L$(BREW_PREFIX)/lib -lglfw -lGLEW -framework OpenGL

TARGETS := task1_red_triangle task1_blue_square task2_part1 task2_part2

all: $(TARGETS)

# Generic rule for compiling individual OpenGL programs.
task1_red_triangle: task1_red_triangle.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) task1_red_triangle.cpp -o $@ $(LIBS)

task1_blue_square: task1_blue_square.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) task1_blue_square.cpp -o $@ $(LIBS)

task2_part1: task2_part1.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) task2_part1.cpp -o $@ $(LIBS)

task2_part2: task2_part2.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) task2_part2.cpp -o $@ $(LIBS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean
