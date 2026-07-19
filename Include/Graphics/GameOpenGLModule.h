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

class GameOpenGLModule:public GameRenderModule {
private:
    std::vector<glm::vec3> resultVertices;
    //ゲームのオブジェクト用
    std::map<unsigned int,std::vector<glm::vec3>> vertices = std::map<unsigned int,std::vector<glm::vec3>>();

    bool initialized = false;

    unsigned int VAO, VBO;
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

    void AddVertices(unsigned int object, std::vector<glm::vec3> vertices);

    unsigned int GetShaderProgram();
};
#endif //EUROPARENDER_GAMEOPENGLMODULE_H
