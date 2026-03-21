#pragma once
#include "engine/window.h"
#include <memory>
#include <vector>

class Scene {
public:
    static std::unique_ptr<Window> window;

    static void init(const std::string& scenePath);
    static void run();
    static void terminate();
private:
    static void createEntities(const std::string& jsonPath);

    static float getDeltaTime();

    static std::vector<float> windowColor;

    static int gridSize;
    static float gridStep;
};