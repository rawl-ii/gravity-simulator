#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class object {
public:
    object(float radius, shader &objectShader, const glm::vec3 &color);

    void draw(const glm::vec3 &position, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    static constexpr GLuint stacks = 64;
    static constexpr GLuint sectors = 64;

    const float radius;

    shader &objectShader;
    const glm::vec3 color;

    void createSphere();
};