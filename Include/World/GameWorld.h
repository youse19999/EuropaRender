//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_GAMEWORLD_H
#define EUROPARENDER_GAMEWORLD_H
#include "GameObject.h"
#include <nlohmann/json.hpp>

#include "GameCamera.h"
#include "GameOpenGLModule.h"

class GameWorld:public GameRenderModule {
private:
    GameCamera* curentCamera;

    unsigned int shaderProgram;

    nlohmann::json json;
    std::vector<GameObject*> gameObjects;
public:
    GameWorld(GLFWwindow* window);

    void AddGameObject(GameObject* gameObject);
    void SetCamera(GameCamera* _camera);
    GameCamera& GetCamera();
    void Update();

    void SetShaderProgram(unsigned int shaderProgram);

    void Dispose() override;
    void PollEvent() override;
    void Init() override;
    void Render() override;
};
#endif //EUROPARENDER_GAMEWORLD_H
