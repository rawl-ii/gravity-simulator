#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 camera::cameraPosition = glm::vec3(0.0f);

glm::vec3 camera::cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera::cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

float camera::cameraSpeed = 5.0f;
float camera::sensitivity = 0.05f;

float camera::cameraPitch = 0.0f;
float camera::cameraYaw = 0.0f;

bool camera::cursorEnabled = false;

void camera::updateCameraVectors() {
    glm::vec3 front;

    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void camera::processCursorInput(GLFWwindow* window, double xOffset, double yOffset) {
    static bool firstMouse;
    static float lastX, lastY;

    if(cursorEnabled) {
        firstMouse = true;
        return; 
    }

    if(firstMouse) {
        lastX = xOffset;
        lastY = yOffset;
    }

    float xPos = (xOffset - lastX) * sensitivity;
    float yPos = (yOffset - lastY) * sensitivity;
    lastX = xOffset;
    lastY = yOffset;
    
    cameraYaw = std::min(cameraYaw -= cameraYaw, 89.0f);
    cameraPitch = std::max(cameraPitch += yPos, -89.0f);
}

void camera::processCursorCallback(GLFWwindow* window) {
    if(cursorEnabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, processCursorInput);
    }
}

void camera::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    float cameraVelocity = cameraSpeed * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W)) {
        cameraPosition += cameraFront * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_S)) {
        cameraPosition -= cameraFront * cameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_A)) {
        cameraPosition -= cameraRight * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_D)) {
        cameraPosition += cameraRight * cameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_Q)) {
        cameraPosition += cameraUp * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_E)) {
        cameraPosition -= cameraUp * cameraVelocity;
    }
}

glm::mat4 camera::getViewMatrix() {
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp); 
}
glm::mat4 camera::getProjectionMatrix(float fovy, float windowWidth, float windowHeight, float near, float far) {
    return glm::perspective(glm::radians(55.0f), (windowWidth / windowHeight), near, far);
}

glm::vec3 camera::getCameraPosition() { return cameraPosition; }

void camera::enableCursor() { cursorEnabled = true; }
void camera::disableCursor() { cursorEnabled = false; }