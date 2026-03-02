#pragma once
#include "renderer.h"
#include "physics.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

//TODO: working objects manager
struct physicsArgs {
    glm::vec3 position;
    glm::vec3 velocity;

    float mass;
    float density;
};

class gameObject {
public:
    static void addStar(physicsArgs pArgs, glm::vec3 color, float radius);
    static void addPlanet(physicsArgs pArgs, glm::vec3 color, float radius);

    static void drawStars(const glm::mat4 &view, const glm::mat4 &projection);
    static void drawPlanets(const glm::mat4 &view, const glm::mat4 &projection);

    float getMass();
    float getRadius();

    glm::vec3 getPosition();
private:
    physicsObject physics;
    renderObject render;

    gameObject(physicsArgs pArgs, SHADER_TYPE type, glm::vec3 color, float radius);
    
    static std::vector<std::unique_ptr<gameObject>> stars;
    static std::vector<std::unique_ptr<gameObject>> planets;

    //physics
    const float mass;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    //render
    SHADER_TYPE type;

    const glm::vec3 color;

    //both
    const float radius;
};