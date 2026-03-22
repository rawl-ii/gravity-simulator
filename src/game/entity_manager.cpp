#include "game/entity_manager.h"

// definitions for static members declared in planet_manager.h
std::unique_ptr<SphereRenderer> EntityManager::entityRenderer = nullptr;
std::vector<std::unique_ptr<EntityManager::Entity>> EntityManager::entities;

EntityManager::Entity::Entity(physicsArgs pArgs, glm::vec3 color, ENTITY_TYPE type):
physics(pArgs.position, pArgs.velocity, pArgs.mass, pArgs.density),
color(color),
type(type) {}

void EntityManager::init() {    
    entityRenderer = std::make_unique<SphereRenderer>();
}

void EntityManager::terminate() {
    entities.clear();
    entityRenderer.reset();

    SphereRenderer::terminate();
}   

void EntityManager::addStar(physicsArgs pArgs, glm::vec3 color) {
    entities.emplace_back(new Entity(pArgs, color, ENTITY_TYPE::STAR));
}

void EntityManager::addPlanet(physicsArgs pArgs, glm::vec3 color) {
    entities.emplace_back(new Entity(pArgs, color, ENTITY_TYPE::PLANET));
}

void EntityManager::drawStars(const glm::vec3 &viewerPosition, const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& star : entities) {
        if(star->type == ENTITY_TYPE::STAR) {
            entityRenderer->draw(star->getPosition(), star->getRadius(), star->color, viewerPosition, view, projection);
        }
    }
}

void EntityManager::drawPlanets(const glm::mat4 &view, const glm::mat4 &projection) {    
    // getting data for the planet shader
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

void EntityManager::updatePhysics(float deltaTime) {
    if(entities.size() < 2) { return; }

    // calculate gravitational forces between all pairs of entities
    for(int i = 0; i < entities.size(); i++) {
        for(int j = i + 1; j < entities.size(); j++) {
            Physics &obj1 = entities[i]->physics;
            Physics &obj2 = entities[j]->physics;

            glm::vec3 force = Physics::calculateGravity(obj1, obj2);

            obj1.applyForce(-force);
            obj2.applyForce(force);
        }
    }

    for(auto& entity : entities) {
        entity->physics.applyVelocity(deltaTime);
    }
}

std::vector<float> EntityManager::getMasses() {
    std::vector<float> masses;
    masses.reserve(entities.size());

    for(auto& entity : entities) {
        masses.push_back(entity->getMass());
    }

    return masses;
}

std::vector<float> EntityManager::getRadiuses() {
    std::vector<float> radiuses;
    radiuses.reserve(entities.size());

    for(auto& entity : entities) {
        radiuses.push_back(entity->getRadius());
    }

    return radiuses;
}

std::vector<glm::vec3> EntityManager::getPositions() {
    std::vector<glm::vec3> positions;
    positions.reserve(entities.size());

    for(auto& entity : entities) {
        positions.push_back(entity->getPosition());
    }

    return positions;
}

size_t EntityManager::getEntitiesSize() { return entities.size(); }

float EntityManager::Entity::getMass() { return physics.getMass(); }
float EntityManager::Entity::getRadius() { return physics.getRadius(); }

glm::vec3 EntityManager::Entity::getPosition() { return physics.getPosition(); }