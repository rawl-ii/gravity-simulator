#pragma once
#include "engine/window.h"
#include "engine/camera.h"
#include "engine/physics.h"
#include "game/entity_manager.h"
#include "json_reader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <iostream>

void setPhysicsConstants(std::string& jsonPath);
void createScene(std::string& jsonPath);

class scene {
public:
    static std::unique_ptr<win> window;

    static void init();
    static void run();
    static void terminate();
};