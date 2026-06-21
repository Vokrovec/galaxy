#pragma once
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include <Star.hpp>
#include <vector>
#include <GLFW/glfw3.h>

class Galaxy {
  public:
    Galaxy() = default;
    void generateStars(size_t numberOfStars);
    [[nodiscard]] std::vector<StarGPU> getStars() const;
    size_t getStarCount() const;
    void updateStars(float deltaTime);
  private:
    std::vector<Star> m_stars;
};
