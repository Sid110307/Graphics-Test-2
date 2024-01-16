cmake_minimum_required(VERSION 3.20)
project(GraphicsTest2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE GLVND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(SOURCE_FILES
        src/main.cpp
        src/include/camera.h
        src/include/mesh.h
        src/include/model.h
        src/include/utils.h
        src/include/shader.h
        tests/environments/randomModels/objects.h
        tests/environments/randomModels/world.h)

add_executable(graphicsTest2 ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(FreeType REQUIRED)

target_include_directories(graphicsTest2 PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS}
        /usr/include/freetype2)
target_link_libraries(graphicsTest2 ${OPENGL_LIBRARIES} GLEW::GLEW glfw ${FREETYPE_LIBRARIES} freetype assimp)
target_compile_definitions(graphicsTest2 PUBLIC STB_IMAGE_IMPLEMENTATION
        FRAGMENT_SHADER_PATH="src/lib/fragment.glsl" VERTEX_SHADER_PATH="src/lib/vertex.glsl")

if (APPLE)
    target_link_libraries(graphicsTest2 /usr/local/lib/libassimp.dylib)
endif ()
