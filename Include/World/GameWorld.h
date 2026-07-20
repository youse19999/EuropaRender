//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_GAMEWORLD_H
#define EUROPARENDER_GAMEWORLD_H

#include "GameObject.h"
#include <nlohmann/json.hpp>

#include "GameCamera.h"
#include "GameOpenGLModule.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

class GameWorld:public GameRenderModule {
private:
    static std::shared_ptr<GameWorld> instance;
    static std::mutex m_singleton;

    GameCamera* curentCamera;

    unsigned int shaderProgram;

    unsigned int gameObjectOffset;

    nlohmann::json json;
    std::vector<GameObject*> gameObjects;

    unsigned int currentIndexOffset;
public:

    static std::shared_ptr<GameWorld> GetInstance();

    GameWorld(GLFWwindow* window);

    void AddGameObject(GameObject* gameObject);
    void SetCamera(GameCamera* _camera);
    GameCamera& GetCamera();
    void Update();

    GameObject* GetGameObject(unsigned int index);
    std::vector<GameObject*> GetAllGameObject();

    void SetShaderProgram(unsigned int shaderProgram);

    void Dispose() override;
    void PollEvent() override;
    void Init() override;
    void Render() override;
};
#endif //EUROPARENDER_GAMEWORLD_H
