//
// Created by youse on 2026/07/18.
//


#ifndef EUROPARENDER_GAMEOPENGLMODULE_H
#define EUROPARENDER_GAMEOPENGLMODULE_H
#include <pch.h>

#include "GameRenderModule.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>

#include "ImGUIRenderModule.h"
#include "Vertex.h"

class GameOpenGLModule:public GameRenderModule {
private:
    unsigned int vertexCount = 0;

    std::vector<Vertex> resultVertices;
    std::vector<unsigned int> resultIndices;
    //ゲームのオブジェクト用
    std::vector<std::vector<Vertex>> vertices = std::vector<std::vector<Vertex>>();

    bool initialized = false;

    unsigned int VAO, VBO,EBO;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

    std::vector<ImGUIRenderModule*> imguiModules;
public:
    GameOpenGLModule(GLFWwindow* window);
    void Dispose() override;
    void PollEvent() override;
    void Init() override;
    void Render() override;

    void AddVertices(unsigned int object, std::vector<Vertex> vertices);
    void AddIndices(std::vector<unsigned int> _indices);

    unsigned int GetShaderProgram();
};
#endif //EUROPARENDER_GAMEOPENGLMODULE_H
