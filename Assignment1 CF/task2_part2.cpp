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

static std::vector<float> makeHexagon(float cx, float cy, float radius, float r, float g, float b)
{
    std::vector<float> data;
    data.push_back(cx);
    data.push_back(cy);
    data.push_back(r);
    data.push_back(g);
    data.push_back(b);

    for (int i = 0; i <= 6; ++i)
    {
        float angle = -static_cast<float>(M_PI) / 2.0f + i * (2.0f * static_cast<float>(M_PI) / 6.0f);
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        data.push_back(x);
        data.push_back(y);
        data.push_back(r);
        data.push_back(g);
        data.push_back(b);
    }
    return data;
}

static std::vector<float> makeStar(float cx, float cy, float outerR, float innerR)
{
    std::vector<float> data;
    for (int i = 0; i < 10; ++i)
    {
        float angle = -static_cast<float>(M_PI) / 2.0f + i * (static_cast<float>(M_PI) / 5.0f);
        float radius = (i % 2 == 0) ? outerR : innerR;
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        data.push_back(x);
        data.push_back(y);
        data.push_back((i % 2 == 0) ? 1.0f : 0.8f);
        data.push_back((i % 2 == 0) ? 0.75f : 0.2f);
        data.push_back((i % 2 == 0) ? 0.2f : 0.9f);
    }
    return data;
}

static std::vector<float> makeRing(float cx, float cy, float radius, float thickness)
{
    std::vector<float> data;
    for (int i = 0; i <= 40; ++i)
    {
        float angle = (static_cast<float>(i) / 40.0f) * 2.0f * static_cast<float>(M_PI);
        float r1 = radius - thickness;
        float r2 = radius;

        float x1 = cx + r1 * std::cos(angle);
        float y1 = cy + r1 * std::sin(angle);
        float x2 = cx + r2 * std::cos(angle);
        float y2 = cy + r2 * std::sin(angle);

        data.push_back(x1);
        data.push_back(y1);
        data.push_back(0.8f);
        data.push_back(0.8f);
        data.push_back(0.3f);

        data.push_back(x2);
        data.push_back(y2);
        data.push_back(0.4f);
        data.push_back(0.8f);
        data.push_back(0.9f);
    }
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

    GLFWwindow* window = glfwCreateWindow(500, 500, "Task 2 Part 2", nullptr, nullptr);
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

    std::vector<float> vertices;
    auto hex = makeHexagon(-0.35f, 0.15f, 0.28f, 1.0f, 0.5f, 0.2f);
    vertices.insert(vertices.end(), hex.begin(), hex.end());

    auto star = makeStar(0.38f, 0.15f, 0.22f, 0.11f);
    vertices.insert(vertices.end(), star.begin(), star.end());

    auto ring = makeRing(-0.05f, -0.3f, 0.24f, 0.08f);
    vertices.insert(vertices.end(), ring.begin(), ring.end());

    unsigned int vao;
    unsigned int vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int program = createProgram();
    glUseProgram(program);
    glClearColor(0.08f, 0.08f, 0.15f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
        glDrawArrays(GL_TRIANGLE_FAN, 7, 10);
        glDrawArrays(GL_TRIANGLE_STRIP, 17, 82);

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
