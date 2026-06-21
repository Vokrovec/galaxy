#pragma once

#include <string>
#include <Galaxy.hpp>
#include <Camera.hpp>
#include <Star.hpp>
#include <GLFW/glfw3.h>


class Renderer {
public:
    Renderer(int width, int height, const std::string & title, Galaxy & Galaxy);
    ~Renderer();
    void Draw();
    void HandleInput();
    void ProcessMouse(int, int);
    void PollEvents();
    bool ShouldClose();
    void setShouldClose(bool);
    void addGalaxy(const Galaxy &);
    void addStars() const;


private:
    int lastX = 0;
    int lastY = 0;
    //int m_StarCount;
    Galaxy & m_galaxy;
    GLFWwindow* window;

    GLuint m_ShaderProgram;
    GLuint m_VAO;
    GLuint m_VBO;
    Camera camera = Camera();
};
