cmake_minimum_required(VERSION 3.2)
project(graphicsTest2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pedantic -g -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter")

set(PROJECT_SOURCE_DIR src)
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp)

add_executable(graphicsTest2 ${SOURCE_FILES})
target_link_libraries(graphicsTest2 GL GLU glut GLEW assimp stb)
