//#pragma once
#include "Galaxy.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Renderer.hpp>
#include <stdexcept>
#include <Star.hpp>
#include <vector>

const char* vertexShaderSrc = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

uniform mat4 view;
uniform mat4 projection;


void main()
{
    vColor = aColor;
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_PointSize = 5.0;
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core

in vec4 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vColor;
}
)";

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Renderer* renderer = (Renderer*)glfwGetWindowUserPointer(window);
    renderer->ProcessMouse(xpos, ypos);
}

//needs to be here, bcs its C lib
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint CompileShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
      char infoLog[512];
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      throw std::runtime_error(infoLog);
  }

  return shader;
}


Renderer::Renderer(int width, int height, const std::string & title, Galaxy & galaxy): m_galaxy(galaxy) {
  // Initialize GLFW
  if (!glfwInit()) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window\n");
  }
  // Configure GLFW to use OpenGL 3.3 Core Profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for macOS
#endif

  window = glfwCreateWindow(800, 600, title.c_str(), nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window\n");
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


  // Initialize GLAD (Load all OpenGL function pointers)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window\n");
  }

  //enable mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, mouse_callback);

  //shaders magic
  GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
  GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

  //shader setup
  m_ShaderProgram = glCreateProgram();

  glAttachShader(m_ShaderProgram, vs);
  glAttachShader(m_ShaderProgram, fs);

  glLinkProgram(m_ShaderProgram);

  GLint success;
  glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);

  if (!success)
  {
      char infoLog[512];
      glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
      throw std::runtime_error(infoLog);
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);

  glDeleteShader(vs);
  glDeleteShader(fs);

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // position
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Star),
      (void*)offsetof(Star, position)
  );
  glEnableVertexAttribArray(0);

  // color
  glVertexAttribPointer(
      1,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Star),
      (void*)offsetof(Star, color)
  );
  glEnableVertexAttribArray(1);


  glBindVertexArray(0);

}
Renderer::~Renderer() {
    glfwTerminate();
}

void Renderer::HandleInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    setShouldClose(true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.Pos += camera.Speed * camera.Front;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.Pos -= camera.Speed * camera.Front;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.Pos -= glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.Speed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.Pos += glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.Speed;
  }
}

// Added this to decouple window logic from rendering commands
void Renderer::PollEvents() {
    glfwPollEvents();
}

bool Renderer::ShouldClose() {
    return glfwWindowShouldClose(window);
}
void Renderer::setShouldClose(bool val) {
    glfwSetWindowShouldClose(window, val);
}

void Renderer::Draw() {
  addStars();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glUseProgram(m_ShaderProgram);

  glm::mat4 projection = glm::perspective(
      glm::radians(60.0f),
      800.0f / 600.0f,
      0.1f,
      100.0f
  );
  GLuint projLoc = glGetUniformLocation(m_ShaderProgram, "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view = glm::lookAt(
      camera.Pos,
      camera.Pos + camera.Front,
      camera.Up
  );

  GLuint viewLoc = glGetUniformLocation(m_ShaderProgram, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


  glBindVertexArray(m_VAO);
  glDrawArrays(GL_POINTS, 0, m_galaxy.getStarCount());

  glfwSwapBuffers(window);
}

void Renderer::addGalaxy(const Galaxy & galaxy) {
  m_galaxy = galaxy;
  addStars();
}

void Renderer::addStars() const {
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  const std::vector<Star> & stars = m_galaxy.getStars();

  glBufferData(GL_ARRAY_BUFFER,
    stars.size() * sizeof(Star),
    stars.data(),
    GL_STATIC_DRAW);
}

void Renderer::ProcessMouse(int X, int Y) {
  float xoffset = X - lastX;
  float yoffset = lastY - Y; // reversed (important)

  lastX = X;
  lastY = Y;

  xoffset *= camera.Sensitivity;
  yoffset *= camera.Sensitivity;

  camera.Yaw += xoffset;
  camera.Pitch += yoffset;

  // clamp pitch to avoid flipping
  if (camera.Pitch > 89.0f)
      camera.Pitch = 89.0f;
  if (camera.Pitch < -89.0f)
      camera.Pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
  direction.y = sin(glm::radians(camera.Pitch));
  direction.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));

  camera.Front = glm::normalize(direction);

}
