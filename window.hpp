#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include <random>

#include "camera.hpp"
#include "ground.hpp"
#include "model.hpp"
#include "car.hpp"

enum class RotationDirection { Right, Left, None };

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  std::default_random_engine m_randomEngine;
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  Model m_model;
  Ground m_ground;

  struct Player {
    glm::vec3 m_position{};
    glm::vec3 m_rotationAxis{};
  };

  struct ParkedCar {
    glm::vec3 m_position{};
    float m_angle{};
    std::array<float, 3> rgb;
  };

  struct MovingCar {
    glm::vec3 m_position{};
    std::array<float, 3> rgb;
  };

  

  std::array<ParkedCar, 6> parkedCars;

  Car car;
  MovingCar movingCar;
  Player player;
  glm::vec3 driveDirection{};
  glm::vec3 driveFront{};
  RotationDirection rotationDirection{ RotationDirection::None };
  float m_angle{};
  float carSpeed{};
  float steerSpeed{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
  float generateRandomColor();

};

#endif