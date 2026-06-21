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
  Renderer rend(400, 800, "Galaxy simulator", galaxy);
  galaxy.generateStars(10000);
  while (!rend.ShouldClose()) {
    rend.PollEvents(); //handle keyboard events
    rend.HandleInput(); //handle keyboard events
    rend.Draw(); //draws screen
  }
  return 0;
}
