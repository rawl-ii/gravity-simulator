#include "game/scene.h"

std::unique_ptr<win> scene::window = nullptr;

void setPhysicsConstants(std::string& jsonPath) {
    jsonFile reader(jsonPath);

    physicsConstants::GRAVITY = reader.get<float>("GRAVITY");;
    physicsConstants::MIN_DISTANCE = reader.get<float>("MIN_DISTANCE");
    physicsConstants::MIN_MASS = reader.get<float>("MIN_MASS");   
}

void createScene(std::string& jsonPath) {
    jsonFile reader(jsonPath);

    for(auto& item : reader.dataFile) {
        float mass = reader.get<float>("mass");
        float density = reader.get<float>("density");

        std::vector<float> position = reader.get<std::vector<float>>("position");
        std::vector<float> velocity = reader.get<std::vector<float>>("velocity");

        std::vector<float> color = reader.get<std::vector<float>>("color");

        if(reader.get<bool>("isStar") == true) {
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

void scene::init() {
    jsonFile cameraConfig("game/config/camera_config.json");
    jsonFile physicsConfig("game/config/physics_config.json");
    jsonFile windowConfig("game/config/window_config.json");

    std::vector<float> cameraPos = cameraConfig.get<std::vector<float>>("initial_position");
    camera::setInitialPosition(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]));
    
    camera::speed = cameraConfig.get<float>("speed");
    camera::fov = cameraConfig.get<float>("fov");
    camera::nearPlane = cameraConfig.get<float>("near_plane");
    camera::farPlane = cameraConfig.get<float>("far_plane");

    window = std::make_unique<win>(
        windowConfig.get<int>("window_width"),
        windowConfig.get<int>("window_heigth"), 
        windowConfig.get<const char*>("tile")
    );

    physicsConstants::GRAVITY = physicsConfig.get<float>("gravity");
    physicsConstants::MIN_DISTANCE = physicsConfig.get<float>("min_distance");
    physicsConstants::MIN_MASS = physicsConfig.get<float>("min_mass");
}

void scene::run() {
    
}