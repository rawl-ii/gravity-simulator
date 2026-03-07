#include "renderer.h"
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <numbers>
#include <memory>
#include <map>

std::map<ENTITY_TYPE, std::unique_ptr<shader>> renderer::shaderLibrary;

GLuint renderer::VAO = 0;
GLuint renderer::VBO = 0;
GLuint renderer::EBO = 0;
GLsizei renderer::indexCount = 0;
bool renderer::isInitialized = false;

renderer::renderer(ENTITY_TYPE type):
type(type) {

    if(!isInitialized) {
        initShaders();
        initGeometry();
        
        isInitialized = true;
    }
}

void renderer::terminate() {
    glDeleteVertexArrays(1, &VAO);
    
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    shaderLibrary.clear();
    isInitialized = false;
}

void renderer::initGeometry() {
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

void renderer::initShaders() {
    const std::string base = std::filesystem::current_path().string();

    shaderLibrary[ENTITY_TYPE::PLANET] = std::make_unique<shader>
    ((base + "/../src/game/shaders/planet.vert").c_str(),
    (base + "/../src/game/shaders/planet.frag").c_str());

    shaderLibrary[ENTITY_TYPE::STAR] = std::make_unique<shader>
    ((base + "/../src/game/shaders/star.vert").c_str(),
    (base + "/../src/game/shaders/star.frag").c_str());
}

void renderer::createSphere(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
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
        for(int j = 0; j < stacks; j++) {
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

void renderer::draw(glm::vec3 position, float radius, glm::vec3 color, glm::mat4 view, glm::mat4 projection) {
    shader &objectShader = *shaderLibrary[type];
    objectShader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(radius));

    objectShader.setMat4("model", model);   
    objectShader.setMat4("view", view);   
    objectShader.setMat4("projection", projection);

    objectShader.setVec3("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}