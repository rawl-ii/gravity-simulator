#include "game/entity_manager.h"
#include "engine/renderer.h"
#include "engine/physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

// definitions for static members declared in planet_manager.h
std::unique_ptr<renderer> entityManager::starRenderer = nullptr;
std::unique_ptr<renderer> entityManager::planetRenderer = nullptr;

std::vector<entity*> entityManager::objects;
std::vector<std::unique_ptr<entity>> entityManager::stars;
std::vector<std::unique_ptr<entity>> entityManager::planets;

entity::entity(physicsArgs pArgs, glm::vec3 color):
color(color),
physics(pArgs.position, pArgs.velocity, pArgs.mass, pArgs.density) {}

void entityManager::init() {    
    starRenderer = std::make_unique<renderer>();
    planetRenderer = std::make_unique<renderer>();
}

void entityManager::terminate() {
    stars.clear();
    planets.clear();
    objects.clear();

    renderer::terminate();
}

void entityManager::addStar(physicsArgs pArgs, glm::vec3 color) {
    std::unique_ptr<entity> newStar(new entity(pArgs, color));

    objects.push_back(newStar.get());
    stars.push_back(std::move(newStar));
}

void entityManager::addPlanet(physicsArgs pArgs, glm::vec3 color) {
    std::unique_ptr<entity> newPlanet(new entity(pArgs, color));

    objects.push_back(newPlanet.get());
    planets.push_back(std::move(newPlanet));
}

void entityManager::drawStars(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& star : stars) {
        starRenderer->draw(star->getPosition(), star->getRadius(), star->color, view, projection);
    }
}

void entityManager::drawPlanets(const glm::mat4 &view, const glm::mat4 &projection) {    
    std::vector<lightData> shaderData;
    for(auto& star : stars) {
        shaderData.push_back({star->getPosition(), star->color}); 
    }

    for(auto& planet : planets) {
        planetRenderer->draw(planet->getPosition(), planet->getRadius(), planet->color, shaderData, view, projection);
    }
}

void entityManager::updatePhysics(float deltaTime) {
    if(objects.size() < 2) { return; }

    for(int i = 0; i < objects.size(); i++) {
        for(int j = i + 1; j < objects.size(); j++) {
            physicsObject &obj1 = objects[i]->physics;
            physicsObject &obj2 = objects[j]->physics;

            glm::vec3 force = physicsObject::calculateGravity(obj1, obj2);

            obj1.applyForce(-force);
            obj2.applyForce(force);
        }
    }

    for(auto& object : objects) {
        object->physics.applyVelocity(deltaTime);
    }
}

float entity::getMass() { return physics.getMass(); }
float entity::getRadius() { return physics.getRadius(); }

glm::vec3 entity::getPosition() { return physics.getPosition(); }