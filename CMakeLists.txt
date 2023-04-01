cmake_minimum_required(VERSION 3.2)
project(graphicsTest2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_SOURCE_DIR src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SOURCE_FILES
	${PROJECT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/include/camera.h
	${PROJECT_SOURCE_DIR}/include/mesh.h
	${PROJECT_SOURCE_DIR}/include/model.h
	${PROJECT_SOURCE_DIR}/include/object.h
	${PROJECT_SOURCE_DIR}/include/shader.h
	tests/environments/randomModels/objects.h
	tests/environments/randomModels/world.h
)

add_executable(graphicsTest2 ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(FreeType REQUIRED)

target_link_libraries(graphicsTest2 GL GLU glfw3 GLEW assimp stb_image freetype)
