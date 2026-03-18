#include "game/scene.h"
#include "game/entity_manager.h"
#include <game/grid_manager.h>
#include "engine/camera.h"
#include "engine/json_reader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

std::unique_ptr<win> Scene::window = nullptr;
std::vector<float> Scene::windowColor;

void Scene::createEntities(const std::string& jsonPath) {
    JsonFile reader(jsonPath);

    for(auto& item : reader.dataFile) {
        float mass = item.at("mass").get<float>();
        float density = item.at("density").get<float>();

        std::vector<float> position = item.at("position").get<std::vector<float>>();
        std::vector<float> velocity = item.at("velocity").get<std::vector<float>>();
        std::vector<float> color = item.at("color").get<std::vector<float>>();

        if(item.at("isStar").get<bool>() == true) {
            EntityManager::addStar({
                glm::vec3(position[0], position[1], position[2]),
                glm::vec3(velocity[0], velocity[1], velocity[2]),
                mass,
                density
            }, glm::vec3(color[0], color[1], color[2]));
        }
        else {
            EntityManager::addPlanet({
                glm::vec3(position[0], position[1], position[2]),
                glm::vec3(velocity[0], velocity[1], velocity[2]),
                mass,
                density
            }, glm::vec3(color[0], color[1], color[2]));
        }
    }
}

float Scene::getDeltaTime() {
    static float deltaTime = 0;
    static float lastFrame = 0;

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    return deltaTime;
}

void Scene::init(const std::string& ScenePath) {
    JsonFile CameraConfig("game/config/camera_config.json");
    JsonFile graphicConfig("game/config/render_config.json");
    JsonFile physicsConfig("game/config/physics_config.json");
    JsonFile windowConfig("game/config/window_config.json");

    std::vector<float> CameraPos = CameraConfig.get<std::vector<float>>("initial_position");
    Camera::setInitialPosition(glm::vec3(CameraPos[0], CameraPos[1], CameraPos[2]));

    Camera::speed = CameraConfig.get<float>("speed");
    Camera::scrollSpeed = CameraConfig.get<float>("scroll_speed");
    Camera::fov = CameraConfig.get<float>("fov");
    Camera::nearPlane = CameraConfig.get<float>("near_plane");
    Camera::farPlane = CameraConfig.get<float>("far_plane");

    std::vector<float> ambientValue = graphicConfig.get<std::vector<float>>("ambient");
    SphereRenderer::ambient = glm::vec3(ambientValue[0], ambientValue[1], ambientValue[2]);
    std::vector<float> diffuseValue = graphicConfig.get<std::vector<float>>("diffuse");
    SphereRenderer::diffuse = glm::vec3(diffuseValue[0], diffuseValue[1], diffuseValue[2]);

    physicsConstants::GRAVITY = physicsConfig.get<float>("gravity");
    physicsConstants::MIN_DISTANCE = physicsConfig.get<float>("min_distance");
    physicsConstants::MIN_MASS = physicsConfig.get<float>("min_mass");

    window = std::make_unique<win>(
        windowConfig.get<int>("window_width"),
        windowConfig.get<int>("window_height"), 
        windowConfig.get<std::string>("title")
    );
    windowColor = windowConfig.get<std::vector<float>>("background_color");

    EntityManager::init();
    Grid::init(gridSize, gridStep, EntityManager::getMasses().size());
    createEntities(ScenePath);
}

void Scene::run() {    
    glEnable(GL_DEPTH_TEST);

    Camera::processCursorCallback(window->getWindow());
    Camera::processScrollCallback(window->getWindow());

    while(!window->windowShouldClose()) {
        glClearColor(windowColor[0], windowColor[1], windowColor[2], windowColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float deltaTime = getDeltaTime();

        glm::mat4 view = Camera::getViewMatrix();
        glm::mat4 projection = Camera::getProjectionMatrix(window->getWidth(), window->getHeight());

        Camera::processKeyboardInput(window->getWindow(), deltaTime);

        EntityManager::updatePhysics(deltaTime);

        std::vector<float> masses = EntityManager::getMasses();
        std::vector<glm::vec3> positions  = EntityManager::getPositions();

        EntityManager::drawStars(Camera::getPosition(), view, projection);
        EntityManager::drawPlanets(view, projection);
        Grid::draw(positions, masses, view, projection);

        window->swapBuffers();
        win::pollEvents();
    }
}

void Scene::terminate() {
    Grid::terminate();
    EntityManager::terminate();

    win::terminate();
}