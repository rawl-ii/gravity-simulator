#include "engine/window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void win::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    win* self = static_cast<win*>(glfwGetWindowUserPointer(window));

    self->width = width;
    self->height = height;

    glViewport(0, 0, width, height);
}

win::win(int width, int height, const char* title) {
    this->width = width;
    this->height = height;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void win::open() {
    if(window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Could not create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowUserPointer(window, this);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Could not load GLAD");
    }
}

GLFWwindow* win::getWindow() { return window; }
int win::getWidth() { return width; }
int win::getHeight() { return height; }

bool win::windowShouldClose() { return glfwWindowShouldClose(window); }
void win::swapBuffers() { glfwSwapBuffers(window); }

void win::pollEvents() { glfwPollEvents(); }
void win::terminate() { glfwTerminate(); }