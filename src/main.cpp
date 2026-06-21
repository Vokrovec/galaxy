#include "Galaxy.hpp"
#include <glad/glad.h>
#include <Renderer.hpp>
#include <glm/fwd.hpp>
#include <Camera.hpp>
#include <Star.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>



int main() {
  Galaxy galaxy{};
  static_assert(std::is_standard_layout_v<Star>);
  galaxy.generateStars(300);
  Renderer rend(400, 800, "Galaxy simulator", galaxy);
  while (!rend.ShouldClose()) {
    rend.PollEvents(); //handle keyboard events
    rend.HandleInput(); //handle keyboard events
    galaxy.updateStars(0.001);
    rend.Draw(); //draws screen
  }
  return 0;
}
