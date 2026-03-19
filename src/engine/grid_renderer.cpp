#include "engine/grid_renderer.h"
#include <filesystem>
#include <iostream>

float gridConfig::SOFTENING;
float gridConfig::INTESITY;
float gridConfig::DECAY;

GLuint Grid::VAO;
GLuint Grid::VBO;
GLuint Grid::EBO;

int Grid::division;
int Grid::lines;
float Grid::step;

size_t Grid::indexCount;

std::unique_ptr<Shader> Grid::shader = nullptr;
bool Grid::isInitialized = false;

void Grid::init(int size, float stepSize) {
    if(isInitialized) { return; }
    
    lines = size;
    division = size * 2;
    step = stepSize;

    const std::string basePath = std::filesystem::current_path().string();
    shader = std::make_unique<Shader>(
        (basePath + "/../src/game/shaders/grid.vert").c_str(),
        (basePath + "/../src/game/shaders/grid.frag").c_str()
    );

    createGrid();
    isInitialized = true;
}

void Grid::terminate() {
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    shader.release();
    isInitialized = false;
}

void Grid::createGrid() {  
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for(int i = 0; i <= division; i++) {
        float z = static_cast<float>(i) - static_cast<float>(lines);

        for(int j = 0; j <= division; j++) {
            float x = static_cast<float>(j) - static_cast<float>(lines);

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

void Grid::draw(std::vector<objectsData> objects, const glm::mat4 &view, const glm::mat4 &projection) {
    shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    shader->setInt("objectCount", objects.size());

    shader->setFloat("softening", gridConfig::SOFTENING);
    shader->setFloat("intensity", gridConfig::INTESITY);
    shader->setFloat("decay", gridConfig::DECAY);

    for(int i = 0; i < objects.size(); i++) {
        std::string base = "objects[" + std::to_string(i) + "].";

        shader->setVec3((base + "position").c_str(), objects[i].position);
        shader->setFloat((base + "mass").c_str(), objects[i].weight);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}