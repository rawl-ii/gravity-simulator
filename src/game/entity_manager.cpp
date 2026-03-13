#include "game/entity_manager.h"
#include <glm/gtc/type_ptr.hpp>

// definitions for static members declared in planet_manager.h
std::unique_ptr<renderer> entityManager::entityRenderer = nullptr;
std::vector<std::unique_ptr<entity>> entityManager::entities;

entity::entity(physicsArgs pArgs, glm::vec3 color, ENTITY_TYPE type):
physics(pArgs.position, pArgs.velocity, pArgs.mass, pArgs.density),
color(color),
type(type) {}

void entityManager::init() {    
    entityRenderer = std::make_unique<renderer>();
}

void entityManager::terminate() {
    entities.clear();

    renderer::terminate();
}

void entityManager::addStar(physicsArgs pArgs, glm::vec3 color) {
    entities.emplace_back(new entity(pArgs, color, ENTITY_TYPE::STAR));
}

void entityManager::addPlanet(physicsArgs pArgs, glm::vec3 color) {
    entities.emplace_back(new entity(pArgs, color, ENTITY_TYPE::PLANET));
}

void entityManager::drawStars(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& star : entities) {
        if(star->type == ENTITY_TYPE::STAR) {
            entityRenderer->draw(star->getPosition(), star->getRadius(), star->color, view, projection);
        }
    }
}

void entityManager::drawPlanets(const glm::mat4 &view, const glm::mat4 &projection) {    
    std::vector<lightData> shaderData;
    for(auto& star : entities) {
        if(star->type == ENTITY_TYPE::STAR) {
            shaderData.push_back({star->getPosition(), star->color});
        }
    }

    for(auto& planet : entities) {
        if(planet->type == ENTITY_TYPE::PLANET) {
            entityRenderer->draw(planet->getPosition(), planet->getRadius(), planet->color, shaderData, view, projection);
        }
    }
}

void entityManager::updatePhysics(float deltaTime) {
    if(entities.size() < 2) { return; }

    for(int i = 0; i < entities.size(); i++) {
        for(int j = i + 1; j < entities.size(); j++) {
            physicsObject &obj1 = entities[i]->physics;
            physicsObject &obj2 = entities[j]->physics;

            glm::vec3 force = physicsObject::calculateGravity(obj1, obj2);

            obj1.applyForce(-force);
            obj2.applyForce(force);
        }
    }

    for(auto& entity : entities) {
        entity->physics.applyVelocity(deltaTime);
    }
}

float entity::getMass() { return physics.getMass(); }
float entity::getRadius() { return physics.getRadius(); }

glm::vec3 entity::getPosition() { return physics.getPosition(); }