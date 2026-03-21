#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Controller {
public:
    static float cameraFov;
    static float cameraNearPlane;
    static float cameraFarPlane;

    static float cameraSensitivity;
    static float cameraSpeed;
    static float cameraScrollSpeed;

    static void init(GLFWwindow* win);
    static void reset();

    static void moveCameraAngle();
    static void moveCameraScroll();
    static void moveCameraKeyboard(float deltaTime);

    static glm::vec3 getCameraPosition();
    static void setCameraPosition(const glm::vec3 &newPosition);

    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix(float windowWidth, float windowHeight);
private:
    static GLFWwindow* window;

    static bool cursorEnabled;
    static void changeCursorState();
    static void cameraMouseCallback(GLFWwindow* win, double xOffset, double yOffset);
    static void cameraScrollCallback(GLFWwindow* win, double, double yOffset);
};