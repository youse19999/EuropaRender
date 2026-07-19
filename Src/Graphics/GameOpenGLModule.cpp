//
// Created by youse on 2026/07/18.
//

#include "GameOpenGLModule.h"
#include "GameWindow.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void GameOpenGLModule::Init() {
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cerr << "GLEW Initialization Failed: " << glewGetErrorString(err) << std::endl;
        return;
    }



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (auto vertex : vertices) {
        resultVertices.insert(resultVertices.end(),vertex.second.begin(),vertex.second.end());
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * resultVertices.size(), resultVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // 1. シェーダーのソースコード（GLSL）を用意
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "   gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // オレンジ色\n"
        "}\n\0";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

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
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, resultVertices.size());
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

void GameOpenGLModule::AddVertices(unsigned int object, std::vector<glm::vec3> _vertices) {
    vertices[object] = _vertices;
}

GameOpenGLModule::GameOpenGLModule(GLFWwindow *window) : GameRenderModule(window) {

}