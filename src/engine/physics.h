#pragma once
#include <glm/glm.hpp>

class physicsObject {
public:
    physicsObject(glm::vec3 position, glm::vec3 velocity, float mass, float density);

    float applyForce(const glm::vec3 &force);

    float applyVelocity(float deltaTime);

    float getMass();
    float getRadius();

    glm::vec3 getPosition ();
private:
    float mass;
    float radius;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};