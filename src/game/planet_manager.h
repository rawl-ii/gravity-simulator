#pragma once
#include "renderer.h"
#include "physics.h"
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
    static std::vector<entity*> objects;
    static std::unique_ptr<renderer> starRenderer;
    static std::unique_ptr<renderer> planetRenderer;

    static std::vector<std::unique_ptr<entity>> stars;
    static std::vector<std::unique_ptr<entity>> planets;
};

class entity {
public:
    friend class entityManager;    

    float getMass();
    float getRadius();

    glm::vec3 getPosition();
private:
    entity(physicsArgs pArgs, ENTITY_TYPE type, glm::vec3 color);

    physicsObject physics;

    ENTITY_TYPE type;
    const glm::vec3 color;
};