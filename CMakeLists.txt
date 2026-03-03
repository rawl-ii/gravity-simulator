cmake_minimum_required(VERSION 3.22)
project(gravity_simulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

set(SOURCE_FILES
    src/main.cpp

    src/glad/glad.c

    src/engine/shader.cpp
    src/engine/renderer.cpp
    src/engine/physics.cpp
    src/engine/window.cpp
    src/engine/camera.cpp

    src/game/planet_manager.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine

    ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)