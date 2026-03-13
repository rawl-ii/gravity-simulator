#include "engine/camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 camera::position = glm::vec3(0.0f);

glm::vec3 camera::front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera::right = glm::normalize(glm::cross(front, up));

float camera::speed = 5.0f;
float camera::sensitivity = 0.05f;
float camera::fov = 55.0f;

float camera::nearPlane = 0.1f;
float camera::farPlane = 100.0f;

float camera::pitch = 0.0f;
float camera::yaw = 90.0f;

bool camera::cursorEnabled = false;

void camera::setInitialPosition(glm::vec3 initialPosition) {
    position = initialPosition;
}

void camera::updateVectors() {
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void camera::processCursorInput(GLFWwindow* window, double xOffset, double yOffset) {
    static bool firstMouse = true;
    static float lastX, lastY;
    
    if(cursorEnabled) {
        firstMouse = true;
        return;
    }

    if(firstMouse) {
        lastX = xOffset;
        lastY = yOffset;

        firstMouse = false;
        return;
    }

    float xPos = (lastX - xOffset) * sensitivity;
    float yPos = (lastY - yOffset) * sensitivity;
    
    lastX = xOffset;
    lastY = yOffset;
    
    yaw -= xPos;
    pitch += yPos;
    
    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);

    updateVectors();
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
    float cameraVelocity = speed * deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W)) {
        position += front * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_S)) {
        position -= front * cameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_A)) {
        position -= right * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_D)) {
        position += right * cameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_Q)) {
        position -= up * cameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_E)) {
        position += up * cameraVelocity;
    }
}

glm::mat4 camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up); 
}
glm::mat4 camera::getProjectionMatrix(float windowWidth, float windowHeight) {
    return glm::perspective(glm::radians(fov), (windowWidth / windowHeight), nearPlane, farPlane);
}

glm::vec3 camera::getPosition() { return position; }

void camera::enableCursor() { cursorEnabled = true; }
void camera::disableCursor() { cursorEnabled = false; }