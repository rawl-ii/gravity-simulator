#include "planet_manager.h"
#include "renderer.h"
#include "physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

//TODO: working objects manager
gameObject::gameObject(physicsArgs pArgs, SHADER_TYPE type, glm::vec3 color, float radius): 
position(pArgs.position),
velocity(pArgs.velocity), 
mass(pArgs.mass),
type(type),
color(color),
radius(radius),
physics(position, velocity, mass, pArgs.density),
render(type, radius, color) {}

void gameObject::addStar(physicsArgs pArgs, glm::vec3 color, float radius) {
    stars.push_back(std::make_unique<gameObject>(pArgs, SHADER_TYPE::STAR, color, radius));
}

void gameObject::addPlanet(physicsArgs pArgs, glm::vec3 color, float radius) {
    planets.push_back(std::make_unique<gameObject>(pArgs, SHADER_TYPE::PLANET, color, radius));
}

void gameObject::drawStars(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& star : stars) {
        glm::mat4 model = glm::translate(glm::mat4(0.0f), star->position);
        star->render.draw(star->position, model, view, projection);
    }
}

void gameObject::drawPlanets(const glm::mat4 &view, const glm::mat4 &projection) {
    for(auto& planet : planets) {
        glm::mat4 model = glm::translate(glm::mat4(0.0f), planet->position);
        planet->render.draw(planet->position, model, view, projection);
    }
}

float gameObject::getMass() { return mass; }
float gameObject::getRadius() { return radius; }

glm::vec3 gameObject::getPosition() { return position; }