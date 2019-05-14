cmake_minimum_required(VERSION 2.8)

project (LearnOpenGL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/lib)

add_executable(awesome src/shader.h src/shader.cpp src/main.cpp)
target_link_libraries(awesome opengl32 glfw3 glew32s)