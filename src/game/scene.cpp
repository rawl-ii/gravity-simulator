#include "game/scene.h"
#include "engine/camera.h"
#include "engine/physics.h"
#include "engine/json_reader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

std::unique_ptr<win> scene::window = nullptr;
std::vector<float> scene::windowColor;

void scene::createScene(const std::string& jsonPath) {
    jsonFile reader(jsonPath);

    for(auto& item : reader.dataFile) {
        float mass = item.at("mass").get<float>();
        float density = item.at("density").get<float>();

        std::vector<float> position = item.at("position").get<std::vector<float>>();
        std::vector<float> velocity = item.at("velocity").get<std::vector<float>>();
        std::vector<float> color = item.at("color").get<std::vector<float>>();

        if(item.at("isStar").get<bool>() == true) {
            entityManager::addStar({
                glm::vec3(position[0], position[1], position[2]),
                glm::vec3(velocity[0], velocity[1], velocity[2]),
                mass,
                density
            }, glm::vec3(color[0], color[1], color[2]));
        }
        else {
            entityManager::addPlanet({
                glm::vec3(position[0], position[1], position[2]),
                glm::vec3(velocity[0], velocity[1], velocity[2]),
                mass,
                density
            }, glm::vec3(color[0], color[1], color[2]));
        }
    }
}

float scene::getDeltaTime() {
    static float deltaTime = 0;
    static float lastFrame = 0;

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    return deltaTime;
}

void scene::init(const std::string& scenePath) {
    jsonFile cameraConfig("game/config/camera_config.json");
    jsonFile physicsConfig("game/config/physics_config.json");
    jsonFile windowConfig("game/config/window_config.json");

    std::vector<float> cameraPos = cameraConfig.get<std::vector<float>>("initial_position");
    camera::setInitialPosition(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]));
    
    camera::speed = cameraConfig.get<float>("speed");
    camera::fov = cameraConfig.get<float>("fov");
    camera::nearPlane = cameraConfig.get<float>("near_plane");
    camera::farPlane = cameraConfig.get<float>("far_plane");

    physicsConstants::GRAVITY = physicsConfig.get<float>("gravity");
    physicsConstants::MIN_DISTANCE = physicsConfig.get<float>("min_distance");
    physicsConstants::MIN_MASS = physicsConfig.get<float>("min_mass");

    window = std::make_unique<win>(
        windowConfig.get<int>("window_width"),
        windowConfig.get<int>("window_height"), 
        (windowConfig.get<std::string>("title")).c_str()
    );
    windowColor = windowConfig.get<std::vector<float>>("background_color");

    entityManager::init();
    createScene(scenePath);
}

void scene::run() {    
    glClearColor(windowColor[0], windowColor[1], windowColor[2], windowColor[3]);
    glEnable(GL_DEPTH_TEST);

    camera::processCursorCallback(window->getWindow());

    while(!window->windowShouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float deltaTime = getDeltaTime();

        glm::mat4 view = camera::getViewMatrix();
        glm::mat4 projection = camera::getProjectionMatrix(window->getWidth(), window->getHeight());

        camera::processKeyboardInput(window->getWindow(), deltaTime);

        entityManager::updatePhysics(deltaTime);
        entityManager::drawStars(view, projection);
        entityManager::drawPlanets(view, projection);

        window->swapBuffers();
        win::pollEvents();
    }
}

void scene::terminate() {
    entityManager::terminate();
    win::terminate();
}