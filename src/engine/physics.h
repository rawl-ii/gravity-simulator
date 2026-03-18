#pragma once
#include <glm/glm.hpp>

struct physicsConstants {
    static float GRAVITY;

    static float MIN_DISTANCE;
    
    static float MIN_MASS;
    static float MIN_DENSITY;
};

class Physics {
public:
    Physics(glm::vec3 position, glm::vec3 velocity, float mass, float density);

    static glm::vec3 calculateGravity(const Physics& obj1, const Physics& obj2);
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