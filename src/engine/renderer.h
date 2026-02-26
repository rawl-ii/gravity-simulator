#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class object {
public:
    object(shader &objectShader, float radius, const glm::vec3 &color);
    static void initGeometry(float radius, shader &objectShader, const glm::vec3 &color);

    void draw(const glm::vec3 &position, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
private:
    static bool isInitialized;

    //since every sphere has the same geometry, its possible to use static VAO's, VBO's and EBO's
    static GLuint VAO, VBO, EBO;
    static GLsizei indexCount;

    static constexpr GLuint stacks = 64;
    static constexpr GLuint sectors = 64;

    const float radius;

    shader &objectShader;
    const glm::vec3 color;

    static void createSphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
};