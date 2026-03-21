#include "engine/keyboard_manager.h"

std::array<bool, GLFW_KEY_LAST> Keyboard::keys;
std::array<bool, GLFW_KEY_LAST> Keyboard::keysChanged;

void Keyboard::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
}

void Keyboard::reset() {
    std::fill(std::begin(keysChanged), std::end(keysChanged), false);
}

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UNKNOWN) { return; }
    
    if(action == GLFW_PRESS) {
        keys[key] = true;
        keysChanged[key] = true; 
    }
    else if(action == GLFW_RELEASE) {
        keys[key] = false;
        keysChanged[key] = false; 
    }
}

bool Keyboard::isKeyDown(int key) { return keys[key]; }
bool Keyboard::isKeyPressed(int key) { return keysChanged[key]; }
bool Keyboard::isKeyReleased(int key) { return !keys[key]; }