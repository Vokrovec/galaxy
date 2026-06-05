#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <Camera.hpp>
#include <Star.hpp>


class Renderer {
public:
    Renderer(int width, int height, const std::string & title);
    ~Renderer();
    void Draw();
    void HandleInput();
    void ProcessMouse(int, int);
    void PollEvents();
    bool ShouldClose();
    void setShouldClose(bool);
    void addStars(const std::vector<star> &);


private:
    int lastX = 0;
    int lastY = 0;
    int m_StarCount;
    GLFWwindow* window;

    GLuint m_ShaderProgram;
    GLuint m_VAO;
    GLuint m_VBO;
    Camera camera = Camera();
};
