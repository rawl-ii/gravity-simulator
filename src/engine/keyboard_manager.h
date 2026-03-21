#include <GLFW/glfw3.h>
#include <array>

class Keyboard {
public:
    static void init(GLFWwindow* window);
    static void reset();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static bool isKeyDown(int key);
    static bool isKeyPressed(int key);
    static bool isKeyReleased(int key);
private:
    static std::array<bool, GLFW_KEY_LAST> keys;
    static std::array<bool, GLFW_KEY_LAST> keysChanged;
};