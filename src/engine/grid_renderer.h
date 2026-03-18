#pragma once
#include "shader.h"
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <memory>

struct objectsData {
    std::vector<glm::vec3> position;
    std::vector<float> weight;
};

class GridRenderer {
public:
    static void init(int size, int stepSize, size_t numObjects);
    static void terminate();

    static void draw(objectsData objects, const glm::mat4 &view, const glm::mat4 &projection);

    static int division;
    static int lines;
    static float step;
private:
    static bool isInitialized;

    static GLuint VAO, VBO, EBO;
    static std::unique_ptr<Shader> shader;

    static size_t objectCount;
    static size_t indexCount;

    static void createGrid();
};