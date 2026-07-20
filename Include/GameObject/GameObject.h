//
// Created by youse on 2026/07/19.
//

#ifndef EUROPARENDER_GAMEOBJECT_H
#define EUROPARENDER_GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>
#include <pch.h>

#include "Vertex.h"
#include "VirtualTexture.h"



extern unsigned int lastIndexPosition;

class GameObject {
private:

    VirtualTexture* texture;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
protected:
    bool textureHasLoaded = false;

    glm::vec3 position = glm::vec3(0.0,0.0,0.0);
    glm::vec3 rotation = glm::vec3(0.0,0.0,0.0);
    glm::vec3 scale = glm::vec3(1.0,1.0,1.0);
public:
    virtual ~GameObject();
    virtual nlohmann::json GetJson();
    virtual std::vector<unsigned int> GetIndices();
    virtual std::vector<Vertex> GetMeshes();
    virtual void Action() = 0;
    virtual void LoadMeshes(std::string path);
    glm::mat4 GetModelMatrix();
    VirtualTexture* GetTexture();
    void SetTexture(VirtualTexture* texture);
    bool GetTextureHasLoaded();
};

#endif //EUROPARENDER_GAMEOBJECT_H
