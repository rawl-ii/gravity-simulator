#include "planet_manager.h"
#include "renderer.h"
#include "physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

gameObject::gameObject(physicsArgs pArgs, SHADER_TYPE type, glm::vec3 color): 
type(type),
color(color),
physics(pArgs.position, pArgs.velocity, pArgs.mass, pArgs.density),
render(type, physics.getRadius(), color) {}

void planetManager::addStar(physicsArgs pArgs, glm::vec3 color, float radius) {
    auto newStar = std::make_unique<gameObject>(pArgs, SHADER_TYPE::STAR, color);

    stars.push_back(std::move(newStar));
    objects.push_back(newStar.get());
}

void planetManager::addPlanet(physicsArgs pArgs, glm::vec3 color, float radius) {
    auto newPlanet = std::make_unique<gameObject>(pArgs, SHADER_TYPE::PLANET, color);

    planets.push_back(std::move(newPlanet));
    objects.push_back(newPlanet.get());
}

void planetManager::drawStars(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& star : stars) {
        glm::mat4 model = glm::translate(glm::mat4(0.0f), star->getPosition());
        star->render.draw(star->getPosition(), model, view, projection);
    }
}

void planetManager::drawPlanets(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& planet : planets) {
        glm::mat4 model = glm::translate(glm::mat4(0.0f), planet->getPosition());
        planet->render.draw(planet->getPosition(), model, view, projection);
    }
}

void planetManager::updatePhysics(float deltaTime, const glm::vec3 force) {
    for(auto& object : objects) {
        object->physics.applyForce(force);
        object->physics.applyVelocity(deltaTime);
    }
}

float gameObject::getMass() { return physics.getMass(); }
float gameObject::getRadius() { return physics.getRadius(); }

glm::vec3 gameObject::getPosition() { return physics.getPosition(); }