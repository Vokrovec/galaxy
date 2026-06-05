#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Camera {

  glm::vec3 Pos   = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Up    = glm::vec3(0.0f, 1.0f, 0.0f);
  float Yaw = -90.0f;   // looking forward
  float Pitch = 0.0f;

  float Sensitivity = 0.1f;

  float Speed = 0.05f;
};
