#ifndef CAR_HPP_
#define CAR_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

class Car {
public:
//   void computeViewMatrix();
//   void computeProjectionMatrix(glm::vec2 const &size);

  void accelerate(float speed);
  void steer(float speed);

//   glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
//   glm::mat4 const &getProjMatrix() const { return m_projMatrix; }
  glm::vec3 car_at{0.0f, 0.2f, 2.5f};
  glm::vec3 car_pos{0.0f, 0.2f, 0.0f};
  float car_angle = glm::radians(180.0f);

private:
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

//   // Matrix to change from world space to camera space
//   glm::mat4 m_viewMatrix;

//   // Matrix to change from camera space to clip space
//   glm::mat4 m_projMatrix;
};

#endif