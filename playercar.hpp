#ifndef PLAYER_CAR_HPP_
#define PLAYER_CAR_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

class PlayerCar {
public:
  void accelerate(float speed);
  void steer(float speed);

  glm::vec3 car_at{0.2f, 0.2f, 2.5f};
  glm::vec3 car_pos{0.2f, 0.2f, 0.0f};
  float car_angle = glm::radians(180.0f);

private:
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};
};

#endif