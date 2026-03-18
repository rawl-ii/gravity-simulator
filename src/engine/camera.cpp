#include "engine/camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::position = glm::vec3(0.0f);

glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::right = glm::normalize(glm::cross(front, up));

float Camera::speed = 5.0f;
float Camera::scrollSpeed = 25.0f;
float Camera::sensitivity = 0.05f;
float Camera::fov = 55.0f;

float Camera::nearPlane = 0.1f;
float Camera::farPlane = 100.0f;

float Camera::pitch = 0.0f;
float Camera::yaw = 90.0f;

bool Camera::cursorEnabled = false;

void Camera::setInitialPosition(glm::vec3 initialPosition) {
    position = initialPosition;
}

void Camera::updateVectors() {
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime) {
    float CameraVelocity;

    glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? CameraVelocity = speed / 4 : CameraVelocity = speed; 
    CameraVelocity *= deltaTime;

    if(glfwGetKey(window, GLFW_KEY_W)) {
        position += front * CameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_S)) {
        position -= front * CameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_A)) {
        position -= right * CameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_D)) {
        position += right * CameraVelocity;
    }

    if(glfwGetKey(window, GLFW_KEY_Q)) {
        position -= up * CameraVelocity;
    }
    else if(glfwGetKey(window, GLFW_KEY_E)) {
        position += up * CameraVelocity;
    }
}

void Camera::processCursorInput(GLFWwindow* window, double xOffset, double yOffset) {
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

void Camera::processCursorCallback(GLFWwindow* window) {
    if(cursorEnabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, processCursorInput);
    }
}

void Camera::processScrollrInput(GLFWwindow* window, double, double yOffset) {
    position += front * scrollSpeed * (float)yOffset;
}

void Camera::processScrollCallback(GLFWwindow* window) {
    glfwSetScrollCallback(window, processScrollrInput);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up); 
}
glm::mat4 Camera::getProjectionMatrix(float windowWidth, float windowHeight) {
    return glm::perspective(glm::radians(fov), (windowWidth / windowHeight), nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() { return position; }

void Camera::enableCursor() { cursorEnabled = true; }
void Camera::disableCursor() { cursorEnabled = false; }