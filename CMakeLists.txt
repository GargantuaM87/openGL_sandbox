cmake_minimum_required(VERSION 3.10.0)
project(newProject VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(newProject 
src/main.cpp
src/shaderClass.cpp
src/VAO.cpp
src/VBO.cpp
src/EBO.cpp
src/glad.h 
src/glad.c)

target_link_libraries(newProject glfw OpenGL::GL)

