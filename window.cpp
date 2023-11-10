#include "window.hpp"

#include <unordered_map>

#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {
  // Camera Movements
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
  
  // Player's Car Movement
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP) 
      carSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_DOWN)
      carSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_LEFT)
      rotationDirection = RotationDirection::Left;
    if (event.key.keysym.sym == SDLK_RIGHT)
      rotationDirection = RotationDirection::Right;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP))
      carSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN))
      carSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT))
      rotationDirection = RotationDirection::None;
    if ((event.key.keysym.sym == SDLK_RIGHT))
      rotationDirection = RotationDirection::None;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");


  m_model.loadObj(assetsPath + "car.obj");
  m_model.setupVAO(m_program);

  parkedCars[0].m_position = glm::vec3(0.5f, 0.2f, -0.5f);
  parkedCars[0].m_angle = 180.0f;
  parkedCars[0].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};

  parkedCars[1].m_position = glm::vec3(0.5f, 0.2f, 0.5f);
  parkedCars[1].m_angle = 180.0f;
  parkedCars[1].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};

  parkedCars[2].m_position = glm::vec3(-0.5f, 0.2f, 0.0f);
  parkedCars[2].m_angle = 0.0f;
  parkedCars[2].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};

  parkedCars[3].m_position = glm::vec3(-0.5f, 0.2f, 0.5f);
  parkedCars[3].m_angle = 0.0f;
  parkedCars[3].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};
  
  parkedCars[4].m_position = glm::vec3(-0.5f, 0.2f, 1.0f);
  parkedCars[4].m_angle = 0.0f;
  parkedCars[4].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};

  parkedCars[5].m_position = glm::vec3(-0.5f, 0.2f, -0.5f);
  parkedCars[5].m_angle = 0.0f;
  parkedCars[5].rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};
  
  movingCar.m_position = glm::vec3(-0.2f, 0.2f, 1.0f);
  movingCar.rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};
}

float Window::generateRandomColor() {
  std::uniform_real_distribution<float> randomColor(0.0f, 1.0f);
  return randomColor(m_randomEngine);
}

void Window::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);


  // Render player car
  glm::mat4 player_model{1.0f};
  player_model = glm::translate(player_model, car.car_pos);
  player_model = glm::rotate(player_model, car.car_angle, glm::vec3(0.0f, 1.0f, 0.0f));
  player_model = glm::scale(player_model, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &player_model[0][0]);
  m_model.render();

  // Render moving car
  glm::mat4 moving_car_model{1.0f};
  moving_car_model = glm::translate(moving_car_model, movingCar.m_position);
  moving_car_model = glm::rotate(moving_car_model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  moving_car_model = glm::scale(moving_car_model, glm::vec3(0.2f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &moving_car_model[0][0]);
  abcg::glUniform4f(m_colorLocation, movingCar.rgb[0], movingCar.rgb[1], movingCar.rgb[2], 1.0f);
  
  m_model.render();

  //Render parked cars
  for (auto &parkedCar : parkedCars) {
    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, parkedCar.m_position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(parkedCar.m_angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // Set uniform variable
    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    abcg::glUniform4f(m_colorLocation, parkedCar.rgb[0], parkedCar.rgb[1], parkedCar.rgb[2], 1.0f);

    m_model.render();
  }


  // Draw ground
  m_ground.paint();
  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};



  if (rotationDirection == RotationDirection::Left) {
    // If reverse, increase turning speed
    if (carSpeed > 0.0f) {
      car.steer(carSpeed * deltaTime * 1.5f);
    } else {
      car.steer(carSpeed * deltaTime);
    }
  } else if (rotationDirection == RotationDirection::Right) {
    // If reverse, increase turning speed
    if (carSpeed > 0.0f) {
      car.steer(-carSpeed * deltaTime * 1.5f);
    } else {
      car.steer(-carSpeed * deltaTime);
    }
  }

  car.accelerate(carSpeed * deltaTime);

  // Increase z by 10 units per second
  movingCar.m_position.z += deltaTime * 10.0f;

  // Reset car position and gen new color
  if (movingCar.m_position.z > 2.0f) {
    movingCar.rgb = {generateRandomColor(), generateRandomColor(), generateRandomColor()};
    movingCar.m_position.z = -10.0f;
  }


  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}