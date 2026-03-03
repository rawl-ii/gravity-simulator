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

class planetManager {
public:
    static void addStar(physicsArgs pArgs, glm::vec3 color);
    static void addPlanet(physicsArgs pArgs, glm::vec3 color);

    static void drawStars(const glm::mat4 &view, const glm::mat4 &projection);
    static void drawPlanets(const glm::mat4 &view, const glm::mat4 &projection);

    static void updatePhysics(float deltaTime, const glm::vec3 force);
private:
    static std::vector<gameObject*> objects;

    static std::vector<std::unique_ptr<gameObject>> stars;
    static std::vector<std::unique_ptr<gameObject>> planets;
};

class gameObject {
public:
    friend class planetManager;    

    float getMass();
    float getRadius();

    glm::vec3 getPosition();
private:
    physicsObject physics;
    renderObject render;

    gameObject(physicsArgs pArgs, SHADER_TYPE type, glm::vec3 color);

    SHADER_TYPE type;
    const glm::vec3 color;
};