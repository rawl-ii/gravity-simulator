#include "window.h"
#include "camera.h"
#include "planet_manager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>
#include <iostream>

float getDeltaTime();
int main() {
    win window(800, 600, "Gravity simulator");
    glEnable(GL_DEPTH_TEST);

    entityManager::init();

    camera::setInitialPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    camera::speed = 75.0f;
    camera::processCursorCallback(window.getWindow());

    entityManager::addStar({glm::vec3(0.0f), glm::vec3(-10.0f, 0.0f, 0.0f), 50.0f, 5.0f}, glm::vec3(1.0f, 0.0f, 0.0f));
    entityManager::addStar({glm::vec3(0.0f, 0.0f, -25.0f), glm::vec3(10.0f, 0.0f, 0.0f), 50.0f, 5.0f}, glm::vec3(1.0f, 1.0f, 0.0f));

    while(!window.windowShouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float deltaTime = getDeltaTime();

        camera::processKeyboardInput(window.getWindow(), deltaTime);

        glm::mat4 view = camera::getViewMatrix();
        glm::mat4 projection = camera::getProjectionMatrix(55.0f, window.getWidth(), window.getHeight(), 0.01f, 2500.0f);

        entityManager::updatePhysics(deltaTime);
        entityManager::drawStars(view, projection);

        window.swapBuffers();
        win::pollEvents();
    }

    entityManager::terminate();
    win::terminate();

    return 0;   
}

float getDeltaTime() {
    static float deltaTime = 0.0f;
    static float lastFrameTime = 0.0f;

    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    return deltaTime;
}