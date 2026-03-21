#include "engine/window.h"
#include <iostream>

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    self->width = width;
    self->height = height;

    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const std::string &title) {
    this->width = width;
    this->height = height;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

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

GLFWwindow* Window::getWindow() { return window; }
int Window::getWidth() { return width; }
int Window::getHeight() { return height; }

bool Window::windowShouldClose() { return glfwWindowShouldClose(window); }
void Window::swapBuffers() { glfwSwapBuffers(window); }

void Window::pollEvents() { glfwPollEvents(); }
void Window::terminate() { glfwTerminate(); }