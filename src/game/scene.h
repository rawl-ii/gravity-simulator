#pragma once
#include "game/entity_manager.h"
#include "engine/window.h"
#include <memory>
#include <vector>
#include <iostream>

class scene {
public:
    static std::unique_ptr<win> window;

    static void init(const std::string& scenePath);
    static void run();
    static void terminate();
private:
    static void createScene(const std::string& jsonPath);
    
    static float getDeltaTime();

    static std::vector<float> windowColor;
};