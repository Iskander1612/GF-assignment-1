# Report

## 1. Project structure and goals

The assignment required a set of C++ programs using modern OpenGL 3.3 Core Profile. The goal was to avoid deprecated fixed-function pipeline calls and instead use shaders, VAO, VBO, and draw calls such as `glDrawArrays` with primitive types like `GL_TRIANGLES` and `GL_TRIANGLE_FAN`.

## 2. Window setup and OpenGL initialization

Each program starts by initializing GLFW and creating a 500x500 window. The context is made current, and then GLEW is initialized. After that, the OpenGL version is checked and the viewport is configured to match the window size.

The `glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);` and `glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);` calls ensure the Core Profile context is created. This avoids legacy OpenGL pipeline behavior.

## 3. Shader pipeline

Each program defines a simple vertex shader and fragment shader. The vertex shader reads:

- `layout(location = 0) in vec2 vPosition;`
- `layout(location = 1) in vec3 vColor;`

It passes color to the fragment shader through `fColor` and writes the position as:

```glsl
gl_Position = vec4(vPosition, 0.0, 1.0);
```

The fragment shader outputs the final pixel color based on the interpolated `fColor` value.

## 4. VAO/VBO and attribute configuration

The geometric data is stored in arrays of floats. For example, each vertex carries position and color. A VAO is bound, then a VBO is filled using `glBufferData`.

Vertex attributes are set with:

```cpp
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
glEnableVertexAttribArray(1);
```

This keeps the data in a modern GPU-friendly format.

## 5. Geometry used in the tasks

### Task 1

- `task1_red_triangle.cpp`: a single red triangle constructed from three vertices and drawn with `GL_TRIANGLES`.
- `task1_blue_square.cpp`: a square is created using a triangle fan of 4 vertices, producing two triangles.

### Task 2 Part 1

- Ellipse: created by sampling angles and scaling the y-coordinate by 0.6.
- Gradient triangle: vertices are generated at different angles, each with RGB colors.
- Red ring: triangle fan with a center vertex and perimeter vertices; red channel varies with angle.
- Nested squares: multiple square layers are drawn around a center, alternating black and white colors.

### Task 2 Part 2

The creative scene combines several objects such as:

- a hexagon,
- a ring or polygon,
- a star-like shape,
- gradient color accents,
- decorative geometry in a modern 500x500 composition.

All objects use vertex data and shader color interpolation.

## 6. Deprecated functions avoided

The project does not use deprecated OpenGL functionality such as:

- `glBegin`
- `glEnd`
- `glVertex*`
- `glColor*`
- fixed-function pipeline rendering

All rendering is done through shader-based pipeline with VAO and VBO.

## 7. Summary

This assignment demonstrates a modern and clean OpenGL workflow that matches the requirements:

- OpenGL 3.3 Core Profile
- C++17
- GLFW + GLEW
- GLSL 330
- VAO/VBO-based rendering
- Shaders with `vPosition` and `vColor`
- Geometry drawn with `GL_TRIANGLES` and `GL_TRIANGLE_FAN`
