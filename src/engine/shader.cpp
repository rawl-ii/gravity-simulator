#include "engine/shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

shader::shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vertexFile, fragmentFile;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch(std::ifstream::failure &error) {
        std::cout << "Could not read shader files: " << error.what() << '\n';
    }

    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();

    GLuint vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Could not compile vertex shader: " << infoLog << '\n';
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Could not compile fragment shader: " << infoLog << '\n';
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "Could not link shaders: " << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

shader::~shader() {
    if(ID != 0) {
        glDeleteProgram(ID);
        ID = 0;
    }
}

void shader::use() {
    glUseProgram(ID);
}

void shader::setBool(const char* name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name), value);   
}

void shader::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void shader::setFloat(const char* name, float value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void shader::setVec2(const char* name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

void shader::setVec3(const char* name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

void shader::setVec4(const char* name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

void shader::setMat2(const char* name, const glm::mat2 &value) {
    glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void shader::setMat3(const char* name, const glm::mat3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void shader::setMat4(const char* name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}