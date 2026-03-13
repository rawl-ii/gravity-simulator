#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class camera {
public:
    static float speed;
    static float sensitivity;
    static float fov;

    static float nearPlane;
    static float farPlane;

    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix(float windowWidth, float windowHeight);

    static glm::vec3 getPosition();
    static void setInitialPosition(glm::vec3 initialPosition);
 
    static void enableCursor();
    static void disableCursor();

    static void processKeyboardInput(GLFWwindow* window, float deltaTime);
    static void processCursorCallback(GLFWwindow* window);
private:
    static glm::vec3 position;

    static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::vec3 up;
    static glm::vec3 front;
    static glm::vec3 right;

    static float pitch, yaw;
    static bool cursorEnabled;

    static void processCursorInput(GLFWwindow* window, double xOffset, double yOffset);
    static void updateVectors();
};