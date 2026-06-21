#include <Galaxy.hpp>
#include <cstddef>
#include <random>

constexpr float softening = 0.01f;

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
    glm::vec3 pos = {Random(-10, 10), Random(-10, 10), Random(-10, 10)};
    glm::vec4 color = {1, 1, 1, 1};
    glm::vec3 velocity = {0, 0, 0};
    m_stars.push_back({
        pos, 
        color, 
        velocity, 
        Random(100.f, 1000.f) //mass
        });
  }
}
std::vector<StarGPU> Galaxy::getStars() const {
  std::vector<StarGPU> stars;
  stars.reserve(m_stars.size());
  for (auto &s : m_stars) {
    stars.push_back({s.position, s.color});
  }
  return stars;
}

size_t Galaxy::getStarCount() const {
  return m_stars.size();
}

void Galaxy::updateStars(float deltaTime) {
  for (size_t i=0; i<m_stars.size(); i++) {
    glm::vec3 acc(0.f);
    for (size_t j=0; j<m_stars.size(); j++) {
      if (i==j) continue;
      glm::vec3 diff = m_stars[j].position - m_stars[i].position;

      float dist2 = std::max(glm::dot(diff, diff), softening);

      float invDist = 1.0f / sqrt(dist2);
      float invDist3 = invDist * invDist * invDist;

      acc += diff * m_stars[j].mass * invDist3;
    }
    m_stars[i].velocity = acc * deltaTime;
  }
  for (auto &s : m_stars)
  {
      s.position += s.velocity * deltaTime;
  }
}
