#include "game/scene.h"
#include "game/controller.h"
#include "game/entity_manager.h"
#include "engine/grid_renderer.h"
#include "engine/json_reader.h"
#include <glm/glm.hpp>

std::unique_ptr<Window> Scene::window = nullptr;
std::vector<float> Scene::windowColor;

void Scene::createEntities(const std::string& jsonPath) {
    // reading the JSON file and creating entities based on it

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
    /* reading data from all the JSON files 
    and intializing core components */

    JsonFile cameraConfig("game/config/camera_config.json");
    JsonFile graphicConfig("game/config/render_config.json");
    JsonFile gridConfig("game/config/grid_config.json");
    JsonFile physicsConfig("game/config/physics_config.json");
    JsonFile windowConfig("game/config/window_config.json");

    Controller::cameraSpeed = cameraConfig.get<float>("speed");
    Controller::cameraScrollSpeed = cameraConfig.get<float>("scroll_speed");
    std::vector<float> CameraPos = cameraConfig.get<std::vector<float>>("initial_position");
    Controller::setCameraPosition(glm::vec3(CameraPos[0], CameraPos[1], CameraPos[2]));

    Controller::cameraFov = cameraConfig.get<float>("fov");
    Controller::cameraSensitivity = cameraConfig.get<float>("sensitivity");
    Controller::cameraNearPlane = cameraConfig.get<float>("near_plane");
    Controller::cameraFarPlane = cameraConfig.get<float>("far_plane");

    std::vector<float> ambientValue = graphicConfig.get<std::vector<float>>("ambient");
    SphereRenderer::ambient = glm::vec3(ambientValue[0], ambientValue[1], ambientValue[2]);
    std::vector<float> diffuseValue = graphicConfig.get<std::vector<float>>("diffuse");
    SphereRenderer::diffuse = glm::vec3(diffuseValue[0], diffuseValue[1], diffuseValue[2]);

    gridConfig::SOFTENING = gridConfig.get<float>("softening");
    gridConfig::INTESITY = gridConfig.get<float>("intensity");
    gridConfig::DECAY = gridConfig.get<float>("decay");

    physicsConstants::GRAVITY = physicsConfig.get<float>("gravity");
    physicsConstants::MIN_DISTANCE = physicsConfig.get<float>("min_distance");
    physicsConstants::MIN_MASS = physicsConfig.get<float>("min_mass");

    window = std::make_unique<Window>(
        windowConfig.get<int>("window_width"),
        windowConfig.get<int>("window_height"), 
        windowConfig.get<std::string>("title")
    );
    windowColor = windowConfig.get<std::vector<float>>("background_color");

    EntityManager::init();
    createEntities(ScenePath);

    Grid::init(gridConfig.get<int>("size"), gridConfig.get<float>("step"));
    Controller::init(window->getWindow());
}

void Scene::run() {    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!window->windowShouldClose()) {
        glClearColor(windowColor[0], windowColor[1], windowColor[2], windowColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float deltaTime = getDeltaTime();

        glm::mat4 view = Controller::getViewMatrix();
        glm::mat4 projection = Controller::getProjectionMatrix(window->getWidth(), window->getHeight());

        Controller::moveCameraKeyboard(deltaTime);
        Controller::moveCameraAngle();

        EntityManager::updatePhysics(deltaTime);

        std::vector<float> masses = EntityManager::getMasses();
        std::vector<glm::vec3> positions  = EntityManager::getPositions();

        EntityManager::drawStars(Controller::getCameraPosition(), view, projection);
        EntityManager::drawPlanets(view, projection);

        std::vector<objectsData> gridData;
        for(int i = 0; i < EntityManager::getEntitiesSize(); i++) {
            /* getting the mass and position from all the entities 
            to give to the grid shader */
            gridData.push_back({positions[i], masses[i]});
        }
        Grid::draw(gridData, view, projection);

        Controller::reset();
        window->swapBuffers();
        Window::pollEvents();
    }
}

void Scene::terminate() {
    Grid::terminate();
    EntityManager::terminate();

    Window::terminate();
}