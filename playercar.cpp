#include "playercar.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void PlayerCar::accelerate(float speed) {
  auto const forward{glm::normalize(car_at - car_pos)};

  car_pos += forward * speed;
  car_at += forward * speed;
}


void PlayerCar::steer(float speed) {
  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, car_pos);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -car_pos);

  car_at = transform * glm::vec4(car_at, 1.0f);
  car_angle += -speed;
}