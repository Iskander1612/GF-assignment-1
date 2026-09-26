# Assignment 1 — Modern OpenGL 3.3 Core Profile

This project contains four C++ OpenGL programs built with modern OpenGL 3.3 Core Profile, GLFW, GLEW, and GLSL 330. The code intentionally avoids deprecated OpenGL calls such as `glBegin`, `glEnd`, `glVertex*`, and `glColor*`.

## Programs

- `task1_red_triangle.cpp` — one red triangle in a 500x500 window.
- `task1_blue_square.cpp` — one blue square in a 500x500 window.
- `task2_part1.cpp` — a 500x500 composition with an ellipse, gradient triangle, red ring, and nested squares.
- `task2_part2.cpp` — a creative composition with multiple objects built from modern OpenGL primitives.

## Dependencies

Install the following packages on Ubuntu/Linux:

```bash
sudo apt update
sudo apt install build-essential libgl1-mesa-dev libglew-dev libglfw3-dev
```

## Build

```bash
make
```

## Run

```bash
./task1_red_triangle
./task1_blue_square
./task2_part1
./task2_part2
```

## Operating system

This project is intended for Linux Ubuntu 22.04 and uses OpenGL 3.3 Core Profile.

## Notes

- VAO and VBO are used for all geometry.
- Shaders use input variables `vPosition` and `vColor`.
- Geometry is drawn with `GL_TRIANGLES` and `GL_TRIANGLE_FAN`.
- The code compiles with C++17.
