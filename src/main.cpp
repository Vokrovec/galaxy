#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <iostream>
#include <Renderer.hpp>
#include <vector>
#include <random>
#include <Camera.hpp>
#include <Star.hpp>


std::vector<star> stars;

float Random(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

int main() {
  Renderer rend(400, 800, "Galaxy simulator");
  for (int i = 0; i < 10000; i++)
  {
      stars.push_back({Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)});
  }
  rend.addStars(stars);
  while (!rend.ShouldClose()) {
    rend.PollEvents(); //handle keyboard events
    rend.HandleInput(); //handle keyboard events

    rend.Draw(); //draws screen
  }
  return 0;
}
