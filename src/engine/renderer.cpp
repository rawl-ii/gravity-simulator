#include "renderer.h"
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <numbers>
#include <memory>
#include <map>

std::map<SHADER_TYPE, std::unique_ptr<shader>> renderObject::shaderLibrary;

GLuint renderObject::VAO = 0;
GLuint renderObject::VBO = 0;
GLuint renderObject::EBO = 0;
GLsizei renderObject::indexCount = 0;
bool renderObject::isInitialized = false;

renderObject::renderObject(SHADER_TYPE type, float radius, const glm::vec3 &color):
type(type), radius(radius), color(color) {

    if(!isInitialized) {
        initShaders();
        initGeometry(radius);
    }

    isInitialized = true;
}

void renderObject::initGeometry(float radius) {
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

void renderObject::initShaders() {
    shaderLibrary[SHADER_TYPE::PLANET] = std::make_unique<shader>("src/game/shaders/planet.vert", "src/game/shaders/planet.frag");
    shaderLibrary[SHADER_TYPE::STAR] = std::make_unique<shader>("src/game/shaders/star.vert", "src/game/shaders/star.frag");
}

void renderObject::createSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
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

void renderObject::draw(const glm::vec3 &position, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    shader &objectShader = *shaderLibrary[type];
    objectShader.use();

    glm::mat4 modelMatrix = glm::translate(model, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));

    objectShader.setMat4("model", modelMatrix);   
    objectShader.setMat4("view", view);   
    objectShader.setMat4("projection", projection);

    objectShader.setVec3("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}