#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class win {
public:
    win(int width, int height, const char* title);
    void open();

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