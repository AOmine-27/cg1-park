  
# CG2 - Park

Projeto 2 para a disciplina de Computação Gráfica da UFABC.
Este projeto consiste em uma renderização de modelos 3D de carros, sendo possível controlar o movimento de um com as setas. Ao mesmo tempo é possível também movimentar a câmera com as teclas WASD.

Todos os carros utilizam o mesmo arquivo obj de modelo, porém cada um é gerado com uma cor aleatória. Há carros estáticos estacionados e também a todo momento um carro passa numa velocidade constante, simulando uma via.

## Funcionamento

A principal classe criada para o funcionamento deste projeto é a `PlayerCar`, representando o carro do usuário e seus movimentos. Fora isso foram adaptadas alguns trechos do projeto base original, Look At, e do Trackball.

### Classe PlayerCar
A classe PlayerCar representa o carro controlado pelo usuário, possuindo funções para calcular seu movimento, tanto aceleração para frente, ré e as curvas.

```
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
```

A movimentação do carro é semelhante a da câmera, tendo dois vetores representando a posição do carro e para onde sua frente aponta, além do angulo do carro, que pode alterar conforme o carro faz uma curva.

---
### Classe Window

Na classe `Window` algumas modificações foram feitas para utilizarmos a classe `Model` e `PlayerCar`.


```
enum class RotationDirection { Right, Left, None };

...

struct Car {
	glm::vec3 m_position{};
	float m_angle{};
	std::array<float, 3> rgb;
};

std::array<Car, 6> parkedCars;

PlayerCar car;
Car movingCar;
RotationDirection rotationDirection{ RotationDirection::None };
float carSpeed{};

...

float generateRandomColor();
```

O struct Car é utilizado tanto para os carros estáticos (array `parkedCars`) quanto o que se movimento, `movingCar`, possuindo os atributos para posição, ângulo e a cor do carro.

Além disso foi criado o enum `RotationDirection` para diferenciar quando o carro está fazendo uma curva e para qual direção e uma variável `carSpeed` para controlar a velocidade do movimento do carro.

Para a aleatoriedade das cores dos carros, a função `generateRandomColor` gera um número float de 0.0 a 1.0.

Em `onEvent()` os inputs foram divididos para tratar tanto o movimento da câmera quanto do carro:
```
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
```

Em `onCreate` utilizamos a classe model para carregar as vértices do carro no arquivo obj e para cada outro carro do cenário é definido a posição, o ângulo do carro e as cores.

```
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
```

Em `onPaint` é feita as transformações das matrizes para cada carro do cenário.
```
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
```

No `onUpdate` é realizado as chamadas das funções de movimentação do carro do usuário e dos carros da outra mão da via, o qual caso ultrapasse a câmera, é resetado a posição e calculado sua nova cor, simulando um outro carro:

```
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
```

## Web
https://aomine-27.github.io/

## Grupo
- 11007015 Daniel Byoung Koo Jung 
- 11062416 André Omine
