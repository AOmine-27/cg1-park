project(park)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp window.cpp model.cpp car.cpp)
enable_abcg(${PROJECT_NAME})
