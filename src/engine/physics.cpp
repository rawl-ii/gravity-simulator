#include "physics.h"
#include <glm/glm.hpp>
#include <cmath>

physicsObject::physicsObject(glm::vec3 position, glm::vec3 velocity, float mass, float density):
position(position), velocity(velocity), acceleration(0.0f), mass(mass) {

    density = std::max(density, 0.00001f);

    float volume = mass / density;
    this->radius = std::cbrt((3.0f * volume) / (4.0f * std::numbers::pi_v<float>));
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