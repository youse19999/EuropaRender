//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_GAMEOBJECT_H
#define EUROPARENDER_GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

#include <pch.h>

class GameObject {
private:
    std::vector<glm::vec3> vertices = std::vector<glm::vec3>{
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f,  0.5f, 0.0f)
    };
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
public:
    virtual ~GameObject();
    virtual nlohmann::json GetJson();
    virtual std::vector<glm::vec3> GetMeshes();
    virtual void Action() = 0;
};

#endif //EUROPARENDER_GAMEOBJECT_H
