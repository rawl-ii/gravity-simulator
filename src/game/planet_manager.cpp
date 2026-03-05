#include "planet_manager.h"
#include "renderer.h"
#include "physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

// definitions for static members declared in planet_manager.h
std::vector<entity*> planetManager::objects;
std::vector<std::unique_ptr<entity>> planetManager::stars;
std::vector<std::unique_ptr<entity>> planetManager::planets;

entity::entity(physicsArgs pArgs, ENTITY_TYPE type, glm::vec3 color): 
type(type),
color(color),
physics(pArgs.position, pArgs.velocity, pArgs.mass, pArgs.density),
render(type, physics.getRadius(), color) {}

void planetManager::terminate() {
    stars.clear();
    planets.clear();

    objects.clear();

    renderer::terminate();
}

void planetManager::addStar(physicsArgs pArgs, glm::vec3 color) {
    std::unique_ptr<entity> newStar(new entity(pArgs, ENTITY_TYPE::STAR, color));

    stars.push_back(std::move(newStar));
    objects.push_back(newStar.get());
}

void planetManager::addPlanet(physicsArgs pArgs, glm::vec3 color) {
    std::unique_ptr<entity> newPlanet(new entity(pArgs, ENTITY_TYPE::PLANET, color));

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

void planetManager::updatePhysics(float deltaTime) {
    if(objects.size() < 2) { return; }
    
    for(int i = 0; i < objects.size(); i++) {
        for(int j = i + 1; j < objects.size(); j++) {
            physicsObject obj1 = objects[i]->physics;
            physicsObject obj2 = objects[j]->physics;

            glm::vec3 force = physicsObject::calculateGravity(obj1, obj2);
            
            obj1.applyForce(force);
            obj2.applyForce(-force);
        }
    }

    for(auto& object : objects) {
        object->physics.applyVelocity(deltaTime);
    }
}

float entity::getMass() { return physics.getMass(); }
float entity::getRadius() { return physics.getRadius(); }

glm::vec3 entity::getPosition() { return physics.getPosition(); }