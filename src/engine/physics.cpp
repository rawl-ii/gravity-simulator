#include "engine/physics.h"
#include <cmath>

float physicsConstants::GRAVITY;
float physicsConstants::MIN_DISTANCE;
float physicsConstants::MIN_MASS;
float physicsConstants::MIN_DENSITY;

Physics::Physics(glm::vec3 position, glm::vec3 velocity, float mass, float density):
position(position), velocity(velocity), acceleration(0.0f), mass(mass) {

    mass = std::max(mass, physicsConstants::MIN_MASS);
    density = std::max(density, physicsConstants::MIN_DENSITY);

    float volume = mass / density;
    this->radius = std::cbrt((3.0f * volume) / (4.0f * std::numbers::pi_v<float>));
}

glm::vec3 Physics::calculateGravity(const Physics& obj1, const Physics& obj2) {
    glm::vec3 direction = obj1.position - obj2.position;
    float distance = glm::length(direction);
    
    if(distance < physicsConstants::MIN_DISTANCE) { return glm::vec3(0.0f); }
    
    float forceMagnitude = (physicsConstants::GRAVITY * obj1.mass * obj2.mass) / (distance * distance);
    glm::vec3 force = glm::normalize(direction) * forceMagnitude;
    
    return force;
}


void Physics::applyForce(const glm::vec3 &force) {
    this->acceleration += force / this->mass;
}

void Physics::applyVelocity(float deltaTime) {
    this->velocity += this->acceleration * deltaTime;
    this->position += this->velocity * deltaTime;

    this->acceleration = glm::vec3(0.0f);
}

float Physics::getRadius() { return this->radius; }
float Physics::getMass() { return this->mass; }

glm::vec3 Physics::getPosition() { return this->position; }