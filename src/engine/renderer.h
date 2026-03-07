#pragma once
#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <memory>

enum class ENTITY_TYPE {
    PLANET,
    STAR  
};

struct lightData {
    glm::vec3 position;
    glm::vec3 color;
};

class renderer {
public:
    renderer();
    static void terminate();

    void draw(glm::vec3 position, float radius, glm::vec3 color, std::vector<lightData> lightSources, glm::mat4 view, glm::mat4 projection);
    void draw(glm::vec3 position, float radius, glm::vec3 color, glm::mat4 view, glm::mat4 projection);
private:
    static bool isInitialized;

    //since every sphere has the same geometry, its possible to use static VAO's, VBO's and EBO's
    static GLuint VAO, VBO, EBO;
    static GLsizei indexCount;

    static constexpr GLuint stacks = 64;
    static constexpr GLuint sectors = 64;

    static std::map<ENTITY_TYPE, std::unique_ptr<shader>> shaderLibrary;

    static void initGeometry();
    static void initShaders();

    static void createSphere(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
};