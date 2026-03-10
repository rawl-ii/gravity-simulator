#pragma once
#include "engine/renderer.h"
#include "engine/physics.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct physicsArgs {
    glm::vec3 position;
    glm::vec3 velocity;

    float mass;
    float density;
};

class entity;
class entityManager {
public:
    static void init();
    static void terminate();

    static void addStar(physicsArgs pArgs, glm::vec3 color);
    static void addPlanet(physicsArgs pArgs, glm::vec3 color);

    static void drawStars(const glm::mat4 &view, const glm::mat4 &projection);
    static void drawPlanets(const glm::mat4 &view, const glm::mat4 &projection);

    static void updatePhysics(float deltaTime);
private:
    static std::unique_ptr<renderer> entityRenderer;
    static std::vector<std::unique_ptr<entity>> entities;
};

class entity {
public:
    friend class entityManager;    

    float getMass();
    float getRadius();

    glm::vec3 getPosition();
private:
    entity(physicsArgs pArgs, glm::vec3 color, ENTITY_TYPE type);

    physicsObject physics;

    const glm::vec3 color;
    ENTITY_TYPE type;
};