#include "engine/sphere_renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <cmath>
#include <numbers>
#include <iostream>

glm::vec3 SphereRenderer::ambient;
glm::vec3 SphereRenderer::diffuse;

std::map<ENTITY_TYPE, std::unique_ptr<Shader>> SphereRenderer::shaderLibrary;

GLuint SphereRenderer::VAO;
GLuint SphereRenderer::VBO;
GLuint SphereRenderer::EBO;
GLsizei SphereRenderer::indexCount;
bool SphereRenderer::isInitialized = false;

SphereRenderer::SphereRenderer() {
    if(!isInitialized) {
        initShaders();
        initGeometry();

        isInitialized = true;
    }
}

void SphereRenderer::terminate() {
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    shaderLibrary.clear();
    isInitialized = false;
}

void SphereRenderer::initGeometry() {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    createSphere(vertices, indices);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    indexCount = static_cast<GLsizei>(indices.size());
}

void SphereRenderer::initShaders() {
    const std::string basePath = std::filesystem::current_path().string();

    shaderLibrary[ENTITY_TYPE::PLANET] = std::make_unique<Shader>
    ((basePath + "/../src/game/shaders/planet.vert").c_str(),
    (basePath + "/../src/game/shaders/planet.frag").c_str());

    shaderLibrary[ENTITY_TYPE::STAR] = std::make_unique<Shader>
    ((basePath + "/../src/game/shaders/star.vert").c_str(),
    (basePath + "/../src/game/shaders/star.frag").c_str());
}

void SphereRenderer::createSphere(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    for(int i = 0; i <= stacks; i++) {
        float phi = std::numbers::pi_v<float> * i / stacks;

        for(int j = 0; j <= sectors; j++) {
            float theta = 2.0f * std::numbers::pi_v<float> * j / sectors;

            GLfloat x = sin(phi) * cos(theta);
            GLfloat y = sin(phi) * sin(theta);
            GLfloat z = cos(phi);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for(int i = 0; i < stacks; i++) {
        for(int j = 0; j < sectors; j++) {
            int first = i * (sectors + 1) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void SphereRenderer::draw(glm::vec3 position, float radius, glm::vec3 color, std::vector<lightData> lightSources, glm::mat4 view, glm::mat4 projection) {
    Shader &objectShader = *shaderLibrary[ENTITY_TYPE::PLANET];
    objectShader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));

    objectShader.setMat4("model", model);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);

    objectShader.setVec3("color", color);
    for(int i = 0; i < lightSources.size(); i++) {
        std::string base = "pointLights[" + std::to_string(i) + "].";

        objectShader.setVec3((base + "position").c_str(), lightSources[i].position);
        objectShader.setVec3((base + "ambient").c_str(), ambient);
        objectShader.setVec3((base + "diffuse").c_str(), lightSources[i].color * diffuse);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void SphereRenderer::draw(glm::vec3 position, float radius, glm::vec3 color, glm::vec3 viewerPosition, glm::mat4 view, glm::mat4 projection) {
    Shader &objectShader = *shaderLibrary[ENTITY_TYPE::STAR];
    objectShader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));

    objectShader.setMat4("model", model);
    objectShader.setMat4("view", view);
    objectShader.setMat4("projection", projection);

    objectShader.setVec3("color", color);
    objectShader.setVec3("viewPos", viewerPosition);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}