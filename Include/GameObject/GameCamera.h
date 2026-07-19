//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_GAMECAMERA_H
#define EUROPARENDER_GAMECAMERA_H
#include <pch.h>
#include <GameObject.h>
class GameCamera : public GameObject {
public:
    GameCamera();

    void Action() override;

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void SetPosition(glm::vec3 _position);
    void SetRotation(glm::vec3 _rotation);
    nlohmann::json GetJson() override;
};

#endif //EUROPARENDER_GAMECAMERA_H
