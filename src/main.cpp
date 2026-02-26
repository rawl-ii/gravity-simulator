#include "window.h"
#include <numeric>
#include <iostream>

int main() {
    win window(800, 600, "Gravity simulator");

    while(!window.windowShouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swapBuffers();
        win::pollEvents();
    }

    win::terminate();
    return 0;   
}