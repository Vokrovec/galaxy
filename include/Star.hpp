#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>


struct Star {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 velocity;
    float mass;
};
struct StarGPU {
    glm::vec3 position;
    glm::vec4 color;
};
