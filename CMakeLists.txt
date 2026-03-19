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
    src/engine/sphere_renderer.cpp
    src/engine/grid_renderer.cpp
    src/engine/physics.cpp
    src/engine/window.cpp
    src/engine/camera.cpp
    src/engine/json_reader.cpp

    src/game/entity_manager.cpp
    src/game/scene.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/game

    ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OpenGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CONFIG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/game/config")
set(CONFIG_DEST "${CMAKE_CURRENT_BINARY_DIR}/game/config")

file(MAKE_DIRECTORY ${CONFIG_DEST})

add_custom_target(config_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/game/config
        ${CMAKE_CURRENT_BINARY_DIR}/game/config
    COMMENT "Updating JSON configuration files"
)