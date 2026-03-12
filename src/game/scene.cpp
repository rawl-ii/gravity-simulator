#include "game/scene.h"

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