#include "game/controller.h"
#include "engine/camera.h"
#include "engine/keyboard_manager.h"

// defining static variables declared on controller.h
bool Controller::cursorEnabled = false;
GLFWwindow* Controller::window = nullptr;

float Controller::cameraFov;
float Controller::cameraNearPlane;
float Controller::cameraFarPlane;

float Controller::cameraSensitivity;
float Controller::cameraSpeed;
float Controller::cameraScrollSpeed;

void Controller::init(GLFWwindow* win) {
    window = win;
    Keyboard::init(window);

    // set callbacks for mouse and scroll input
    glfwSetCursorPosCallback(window, cameraMouseCallback);
    glfwSetScrollCallback(window, cameraScrollCallback);

    changeCursorState();
}

void Controller::reset() {
    Keyboard::reset();
}

void Controller::cameraMouseCallback(GLFWwindow* win, double xOffset, double yOffset) {
    static bool firstMovement = true;
    static float lastX, lastY;

    if(cursorEnabled) {
        //if the cursor is enabled, dont move the camera
        firstMovement = true;
        return;
    }

    if(firstMovement) {
        /* this is to make sure that the camera dont jump to a random 
        angle when the game starts, since the 
        mouse position is not at (0, 0) */

        lastX = static_cast<float>(xOffset);
        lastY = static_cast<float>(yOffset);

        firstMovement = false;
        return;
    }

    float xAngle = (lastX - xOffset) * cameraSensitivity;
    float yAngle = (lastY - yOffset) * cameraSensitivity;

    lastX = xOffset;
    lastY = yOffset;

    // sending the data to the actual camera
    Camera::setAngle(xAngle, yAngle);
}

void Controller::changeCursorState() {
    if(cursorEnabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Controller::moveCameraAngle() {
    if(Keyboard::isKeyPressed(GLFW_KEY_ESCAPE)) {
        cursorEnabled = !cursorEnabled;
        changeCursorState();
    }
}

void Controller::cameraScrollCallback(GLFWwindow* win, double, double yOffset) {
    // read scroll input to move the camera
    float velocity = cameraScrollSpeed * static_cast<float>(yOffset);
    Camera::move(Camera::getFront(), velocity);
}

void Controller::moveCameraKeyboard(float deltaTime) {
    // read keyboard input to move the camera
    float velocity = cameraSpeed;

    if(Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        velocity /= 4;
    }
    velocity *= deltaTime;

    if(Keyboard::isKeyDown(GLFW_KEY_W)) {
        Camera::move(Camera::getFront(), velocity);
    }
    else if(Keyboard::isKeyDown(GLFW_KEY_S)) {
        Camera::move(Camera::getFront(), -velocity);
    }

    if(Keyboard::isKeyDown(GLFW_KEY_D)) {
        Camera::move(Camera::getRight(), velocity);
    }
    else if(Keyboard::isKeyDown(GLFW_KEY_A)) {
        Camera::move(Camera::getRight(), -velocity);
    }

    if(Keyboard::isKeyDown(GLFW_KEY_E)) {
        Camera::move(Camera::getUp(), velocity);
    }
    else if(Keyboard::isKeyDown(GLFW_KEY_Q)) {
        Camera::move(Camera::getUp(), -velocity);
    }
}

glm::vec3 Controller::getCameraPosition() { return Camera::position; }

void Controller::setCameraPosition(const glm::vec3 &newPosition) {
    Camera::position = newPosition;
}

glm::mat4 Controller::getViewMatrix() { return Camera::getViewMatrix(); }

glm::mat4 Controller::getProjectionMatrix(float windowWidth, float windowHeigth) {
    return Camera::getProjectionMatrix(windowWidth, windowHeigth, cameraFov, cameraNearPlane, cameraFarPlane);
}