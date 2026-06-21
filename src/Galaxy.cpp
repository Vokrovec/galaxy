#include <Galaxy.hpp>
#include <cstddef>
#include <random>

float Random(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

void Galaxy::generateStars(size_t numberOfStars) {
  m_stars = {};
  m_stars.reserve(numberOfStars);
  for (size_t i=0; i<numberOfStars; i++) {
    glm::vec3 pos = {Random(-1, 1), Random(-1, 1), Random(-1, 1)};
    glm::vec4 color = {Random(0, 1), Random(0, 1), Random(0, 1), 1};
    m_stars.push_back({pos, color});
  }
}
const std::vector<Star> & Galaxy::getStars() const {
  return m_stars;
}

size_t Galaxy::getStarCount() const {
  return m_stars.size();
}
