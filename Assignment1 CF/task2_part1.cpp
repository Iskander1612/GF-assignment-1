#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;
out vec3 fColor;

void main()
{
    gl_Position = vec4(vPosition, 0.0, 1.0);
    fColor = vColor;
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
in vec3 fColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(fColor, 1.0);
}
)";

static void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static unsigned int compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error: " << infoLog << '\n';
        std::exit(1);
    }

    return shader;
}

static unsigned int createProgram()
{
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program link error: " << infoLog << '\n';
        std::exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

static std::vector<float> makeEllipse(float cx, float cy, float rx, float ry, int segments)
{
    std::vector<float> data;
    data.push_back(cx);
    data.push_back(cy);
    data.push_back(1.0f);
    data.push_back(0.0f);
    data.push_back(0.0f);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = (static_cast<float>(i) / static_cast<float>(segments)) * 2.0f * static_cast<float>(M_PI);
        float x = cx + rx * std::cos(angle);
        float y = cy + ry * std::sin(angle);
        data.push_back(x);
        data.push_back(y);
        data.push_back(1.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);
    }
    return data;
}

static std::vector<float> makeTriangleGradient(float x0, float y0, float size)
{
    // Top = red, left = green, right = blue
    const float topX = x0;
    const float topY = y0 + size;
    const float leftX = x0 - size * 0.8f;
    const float leftY = y0 - size * 0.6f;
    const float rightX = x0 + size * 0.8f;
    const float rightY = y0 - size * 0.6f;

    std::vector<float> data = {
        topX, topY, 1.0f, 0.0f, 0.0f,
        leftX, leftY, 0.0f, 1.0f, 0.0f,
        rightX, rightY, 0.0f, 0.0f, 1.0f
    };
    return data;
}

static std::vector<float> makeRedRing(float cx, float cy, float radius, int segments)
{
    std::vector<float> data;
    data.push_back(cx);
    data.push_back(cy);
    data.push_back(1.0f);
    data.push_back(0.0f);
    data.push_back(0.0f);

    for (int i = 0; i <= segments; ++i)
    {
        float angle = (static_cast<float>(i) / static_cast<float>(segments)) * 2.0f * static_cast<float>(M_PI);
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        float red = 0.5f + 0.5f * std::sin(angle + 1.0f);
        data.push_back(x);
        data.push_back(y);
        data.push_back(red);
        data.push_back(0.1f);
        data.push_back(0.1f);
    }
    return data;
}

static std::vector<float> makeSquare(float cx, float cy, float size, float r, float g, float b)
{
    // square points generated as triangle fan with center and 4 corners
    std::vector<float> data = {
        cx, cy, r, g, b,
        cx + size, cy + size, r, g, b,
        cx - size, cy + size, r, g, b,
        cx - size, cy - size, r, g, b,
        cx + size, cy - size, r, g, b,
        cx + size, cy + size, r, g, b
    };
    return data;
}

int main()
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Task 2 Part 1", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << '\n';
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, 500, 500);

    std::vector<float> allVertices;

    auto ellipse = makeEllipse(-0.68f, 0.70f, 0.22f, 0.13f, 48);
    allVertices.insert(allVertices.end(), ellipse.begin(), ellipse.end());

    auto gradientTriangle = makeTriangleGradient(0.0f, 0.70f, 0.26f);
    allVertices.insert(allVertices.end(), gradientTriangle.begin(), gradientTriangle.end());

    auto ring = makeRedRing(0.68f, 0.70f, 0.18f, 48);
    allVertices.insert(allVertices.end(), ring.begin(), ring.end());

    auto q1 = makeSquare(0.0f, -0.28f, 0.36f, 0.0f, 0.0f, 0.0f);
    allVertices.insert(allVertices.end(), q1.begin(), q1.end());
    auto q2 = makeSquare(0.0f, -0.28f, 0.25f, 1.0f, 1.0f, 1.0f);
    allVertices.insert(allVertices.end(), q2.begin(), q2.end());
    auto q3 = makeSquare(0.0f, -0.28f, 0.17f, 0.0f, 0.0f, 0.0f);
    allVertices.insert(allVertices.end(), q3.begin(), q3.end());
    auto q4 = makeSquare(0.0f, -0.28f, 0.10f, 1.0f, 1.0f, 1.0f);
    allVertices.insert(allVertices.end(), q4.begin(), q4.end());

    unsigned int vao;
    unsigned int vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), allVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int program = createProgram();
    glUseProgram(program);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 1 + 49 + 1);
        glDrawArrays(GL_TRIANGLES, 1 + 49 + 1, 3);
        glDrawArrays(GL_TRIANGLE_FAN, 1 + 49 + 1 + 3, 1 + 49 + 1);

        const int offsetSquares = 1 + 49 + 1 + 3 + 1 + 49 + 1;
        glDrawArrays(GL_TRIANGLE_FAN, offsetSquares, 6);
        glDrawArrays(GL_TRIANGLE_FAN, offsetSquares + 6, 6);
        glDrawArrays(GL_TRIANGLE_FAN, offsetSquares + 12, 6);
        glDrawArrays(GL_TRIANGLE_FAN, offsetSquares + 18, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
