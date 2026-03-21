#include "engine/camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::position = glm::vec3(0.0f);

glm::vec3 Camera::front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 Camera::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::right = glm::normalize(glm::cross(front, up));

float Camera::pitch = 0.0f;
float Camera::yaw = 90.0f;

void Camera::updateVectors() {
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::move(const glm::vec3 &lookVector, float velocity) {
    position += lookVector * velocity;
}

void Camera::setAngle(float xAngle, float yAngle) {
    yaw -= xAngle;
    pitch += yAngle;

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);

    updateVectors();
}

glm::vec3 Camera::getFront() { return front; }
glm::vec3 Camera::getUp() { return up; }
glm::vec3 Camera::getRight() { return right; }

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up); 
}
glm::mat4 Camera::getProjectionMatrix(float windowWidth, float windowHeight, float fov, float nearPlane, float farPlane) {
    return glm::perspective(glm::radians(fov), (windowWidth / windowHeight), nearPlane, farPlane);
}