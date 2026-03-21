#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(int width, int height, const std::string &title);

    GLFWwindow* getWindow();

    int getWidth();
    int getHeight();

    bool windowShouldClose();
    void swapBuffers();

    static void pollEvents();
    static void terminate();
private:
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow *window;
    unsigned int width, height;
};