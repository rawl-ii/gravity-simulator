#pragma once
#include <glm/glm.hpp>

class physicsObject {
public:
    static constexpr float GRAVITY = 100.0f;

    physicsObject(glm::vec3 position, glm::vec3 velocity, float mass, float density);

    static glm::vec3 calculateGravity(const physicsObject& obj1, const physicsObject& obj2);
    void applyForce(const glm::vec3 &force);
    void applyVelocity(float deltaTime);

    float getMass();
    float getRadius();

    glm::vec3 getPosition();
private:
    float mass;
    float radius;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};