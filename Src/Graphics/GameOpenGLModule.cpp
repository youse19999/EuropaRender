//
// Created by youse on 2026/07/18.
//

#include "GameOpenGLModule.h"
#include "GameWindow.h"

#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

std::stringstream LOGGER_RENDER;

#include "logger.h"

void GameOpenGLModule::Init() {
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cerr << "GLEW Initialization Failed: " << glewGetErrorString(err) << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (auto vertex : vertices) {
        vertexCount += vertex.size();
        resultVertices.insert(resultVertices.end(),vertex.begin(),vertex.end());
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * resultVertices.size(), resultVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * resultIndices.size(), resultIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Joint));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weight));
    glEnableVertexAttribArray(4);

    // 1. シェーダーのソースコード（GLSL）を用意
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aUV;\n"
        "layout (location = 3) in vec4 aJoint;\n"
        "layout (location = 4) in vec4 aWeight;\n"
        "out vec3 POSITION;\n"
        "out vec3 NORMAL;\n"
        "out vec2 UV;\n"
        "out vec4 JOINT;\n"
        "out vec4 WEIGHT;\n"
        "uniform mat4 jointMatrix[64];\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "void main() {\n"
        "   mat4 skinMatrix =\n"
        "   aWeight.x * jointMatrix[int(aJoint.x)] +\n"
        "   aWeight.y * jointMatrix[int(aJoint.y)] +\n"
        "   aWeight.z * jointMatrix[int(aJoint.z)] +\n"
        "   aWeight.w * jointMatrix[int(aJoint.w)];\n"
        "   gl_Position = projection * view * model * skinMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   UV = aUV;\n"
        "   NORMAL = aNormal;\n"
        "   POSITION = aPos;\n"
        "   JOINT = aJoint;\n"
        "   WEIGHT = aWeight;\n"
        "}\0";

        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform sampler2D texture1;\n"
            "in vec3 POSITION;\n"
            "in vec3 NORMAL;\n"
            "in vec2 UV;\n"
            "in vec4 JOINT;\n"
            "in vec4 WEIGHT;\n"
            "void main() {\n"
            //"   FragColor = WEIGHT;\n"
            "   FragColor = texture(texture1, UV);\n"
            "}\n\0";

    GLint success;
    char infoLog[512];
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);


    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    initialized = true;
}

void GameOpenGLModule::PollEvent() {

}

void GameOpenGLModule::Render() {
    glUseProgram(shaderProgram);
}

unsigned int GameOpenGLModule::GetShaderProgram() {
    return shaderProgram;
}

void GameOpenGLModule::Dispose() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void GameOpenGLModule::AddVertices(unsigned int object, std::vector<Vertex> _vertices) {
    vertices.push_back(_vertices);
}
void GameOpenGLModule::AddIndices(std::vector<unsigned int> _indices) {
    resultIndices.insert(resultIndices.end(),_indices.begin(),_indices.end());
}

GameOpenGLModule::GameOpenGLModule(GLFWwindow *window) : GameRenderModule(window) {

}