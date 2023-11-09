#include "car.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

// void Camera::computeProjectionMatrix(glm::vec2 const &size) {
//   m_projMatrix = glm::mat4(1.0f);
//   auto const aspect{size.x / size.y};
//   m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 5.0f);
// }

// void Camera::computeViewMatrix() {
//   m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
// }

void Car::accelerate(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(car_at - car_pos)};

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  car_pos += forward * speed;
  car_at += forward * speed;
}


void Car::steer(float speed) {
  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, car_pos);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -car_pos);

  car_at = transform * glm::vec4(car_at, 1.0f);
  car_angle += -speed;
}