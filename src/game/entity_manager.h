#pragma once
#include "engine/sphere_renderer.h"
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

class EntityManager {
public:
    static void init();
    static void terminate();

    static void addStar(physicsArgs pArgs, glm::vec3 color);
    static void addPlanet(physicsArgs pArgs, glm::vec3 color);

    static void drawStars(const glm::vec3 &viewerPosition, const glm::mat4 &view, const glm::mat4 &projection);
    static void drawPlanets(const glm::mat4 &view, const glm::mat4 &projection);

    static void updatePhysics(float deltaTime);

    static std::vector<float> getMasses();
    static std::vector<float> getRadiuses();
    static std::vector<glm::vec3> getPositions();
private:
    class Entity {
        friend class EntityManager;  

        float getMass();
        float getRadius();

        glm::vec3 getPosition();
        Entity(physicsArgs pArgs, glm::vec3 color, ENTITY_TYPE type);

        Physics physics;

        const glm::vec3 color;
        ENTITY_TYPE type;
    };

    static std::unique_ptr<SphereRenderer> entityRenderer;
    static std::vector<std::unique_ptr<Entity>> entities;
};