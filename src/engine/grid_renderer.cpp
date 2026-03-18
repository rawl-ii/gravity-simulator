#include "engine/grid_renderer.h"
#include <filesystem>
#include <iostream>

GLuint GridRenderer::VAO;
GLuint GridRenderer::VBO;
GLuint GridRenderer::EBO;
int GridRenderer::division;
int GridRenderer::lines;
float GridRenderer::step;
size_t GridRenderer::objectCount;
size_t GridRenderer::indexCount;
std::unique_ptr<Shader> GridRenderer::shader = nullptr;
bool GridRenderer::isInitialized = false;

void GridRenderer::init(int size, int stepSize, size_t numObjects) {
    if(isInitialized) { return; }
    
    objectCount = numObjects;
    lines = size;
    division = size * 2;
    step = stepSize;

    const std::string basePath = std::filesystem::current_path().string();
    shader = std::make_unique<Shader>(
        (basePath + "/../src/game/shaders/grid.vert").c_str(),
        (basePath + "/../src/game/shaders/grid.frag").c_str()
    );

    shader->use();
    shader->setInt("objectCount", numObjects);

    createGrid();
    isInitialized = true;
}

void GridRenderer::terminate() {
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    isInitialized = false;
}

void GridRenderer::createGrid() {        
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for(int i = 0; i <= division; i++) {
        float z = (float)i - (float)lines;

        for(int j = 0; j <= division; j++) {
            float x = (float)j - (float)lines;

            vertices.push_back(x * step);
            vertices.push_back(0.0f);
            vertices.push_back(z * step);
        }
    }

    int pointsPerLine = division + 1;
    for(int i = 0; i <= division; i++) {
        for(int j = 0; j < division; j++) {
            indices.push_back(i * pointsPerLine + j);
            indices.push_back(i * pointsPerLine + j + 1);
        }
    }

    for (int j = 0; j <= division; j++) {
        for (int i = 0; i < division; i++) {
            indices.push_back(i * pointsPerLine + j);
            indices.push_back((i + 1) * pointsPerLine + j);
        }
    }
    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
};

void GridRenderer::draw(objectsData objects, const glm::mat4 &view, const glm::mat4 &projection) {
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for(int i = 0; i < objectCount; i++) {
        std::string base = "objects[" + std::to_string(i) + "].";

        shader->setVec3((base + "position").c_str(), objects.position[i]);
        shader->setFloat((base + "weigth").c_str(), objects.weight[i]);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}