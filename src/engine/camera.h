#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    static glm::vec3 position;

    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix(float windowWidth, float windowHeight, float fov, float nearPlane, float farPlane);

    static void move(const glm::vec3 &newPosition, float velocity);

    static void setAngle(float xAngle, float yAngle);
    static glm::vec3 getFront();
    static glm::vec3 getUp();
    static glm::vec3 getRight();
private:
    static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::vec3 up;
    static glm::vec3 front;
    static glm::vec3 right;

    static float pitch, yaw;
    
    static void updateVectors();
};