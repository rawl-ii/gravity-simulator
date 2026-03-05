#include "physics.h"
#include <glm/glm.hpp>
#include <cmath>

float physicsObject::GRAVITY = 100.0f; 

physicsObject::physicsObject(glm::vec3 position, glm::vec3 velocity, float mass, float density):
position(position), velocity(velocity), acceleration(0.0f), mass(mass) {

    density = std::max(density, 0.00001f);

    float volume = mass / density;
    this->radius = std::cbrt((3.0f * volume) / (4.0f * std::numbers::pi_v<float>));
}

glm::vec3 physicsObject::calculateGravity(const physicsObject& obj1, const physicsObject& obj2) {
    glm::vec3 direction = obj1.position - obj2.position;
    float distance = glm::length(direction);
    
    if(distance <= 0.01f) { return glm::vec3(0.0f); }
    
    float forceMagnitude = (GRAVITY * obj1.mass * obj2.mass) / (distance * distance);
    glm::vec3 force = glm::normalize(direction) * forceMagnitude;
    
    return force;
}


void physicsObject::applyForce(const glm::vec3 &force) {
    this->acceleration += force / this->mass;
}

void physicsObject::applyVelocity(float deltaTime) {
    this->velocity += this->acceleration * deltaTime;
    this->position += this->velocity * deltaTime;

    this->acceleration = glm::vec3(0.0f);
}

float physicsObject::getRadius() { return this->radius; }
float physicsObject::getMass() { return this->mass; }

glm::vec3 physicsObject::getPosition() { return this->position; }