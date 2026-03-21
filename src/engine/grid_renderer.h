#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct objectsData {
    glm::vec3 position;
    float weight;
};

struct gridConfig {
    static float SOFTENING;
    static float INTESITY;
    static float DECAY;
};

class Grid {
public:
    static void init(int size, float stepSize);
    static void terminate();

    static void draw(std::vector<objectsData> objects, const glm::mat4 &view, const glm::mat4 &projection);

    static int divisions;
    static int lines;
    static float step;
private:
    static bool isInitialized;

    static GLuint VAO, VBO, EBO;
    static std::unique_ptr<Shader> shader;

    static size_t indexCount;

    static void createGrid();
};