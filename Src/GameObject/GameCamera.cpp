//
// Created by youse on 2026/07/19.
//

#include <GameCamera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "GameWorld.h"


glm::mat4 GameCamera::GetViewMatrix() {
    glm::mat4 view = glm::lookAt(this->position,glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
    return view;
}
glm::mat4 GameCamera::GetProjectionMatrix() {
    float fov = glm::radians(45.0f);
    float aspect = 4.0f / 3.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 projection = glm::perspective(fov, aspect, nearPlane, farPlane);

    return projection;
}

nlohmann::json GameCamera::GetJson() {
    nlohmann::json json;
    json["px"] = this->position.x;
    json["pr"] = this->position.r;
    json["pz"] = this->position.z;
    return json;
}

void GameCamera::Action() {
    this->position -= glm::vec3(0.0f,0.0f,0.0005f);
    this->rotation -= glm::vec3(0.0f,0.0f,0.0005f);
}
void GameCamera::SetPosition(glm::vec3 _position) {
    this->position = _position;
}
void GameCamera::SetRotation(glm::vec3 _rotation) {
    this->rotation = _rotation;
}
GameCamera::GameCamera() {

}