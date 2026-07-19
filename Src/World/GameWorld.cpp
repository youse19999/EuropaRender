//
// Created by youse on 2026/07/19.
//

#include "GameWorld.h"

#include <glm/gtc/type_ptr.hpp>

void GameWorld::AddGameObject(GameObject* gameObject)  {
    gameObjects.push_back(gameObject);
}

void GameWorld::SetCamera(GameCamera* _camera) {
    this->curentCamera = _camera;
}

void GameWorld::Update() {
    for (auto obj : gameObjects) {
        obj->Action();
    }
}

void GameWorld::SetShaderProgram(unsigned int _shaderProgram) {
    shaderProgram = _shaderProgram;
}

GameWorld::GameWorld(GLFWwindow* window) : GameRenderModule(window) {
}

void GameWorld::Dispose() {

}
void GameWorld::PollEvent() {

}
void GameWorld::Init() {

}
void GameWorld::Render() {
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(curentCamera->GetViewMatrix()));
    GLint proejctionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(proejctionLoc, 1, GL_FALSE, glm::value_ptr(curentCamera->GetProjectionMatrix()));
}