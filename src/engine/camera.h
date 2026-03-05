#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class camera {
public:
    static float cameraSpeed;
    static float sensitivity;

    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix(float fovy, float windowWidth, float windowHeight, float near, float far);
    static glm::vec3 getCameraPosition();

    static void enableCursor();
    static void disableCursor();

    static void processKeyboardInput(GLFWwindow* window, float deltaTime);
    static void processCursorCallback(GLFWwindow* window);
private:
    static glm::vec3 cameraPosition;

    static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::vec3 cameraUp;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraRight;

    static float cameraPitch, cameraYaw;
    static bool cursorEnabled;

    static void processCursorInput(GLFWwindow* window, double xOffset, double yOffset);
    static void updateCameraVectors();
};