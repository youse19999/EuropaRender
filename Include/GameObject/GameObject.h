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
extern unsigned int gameObjectLastOffset;

class GameObject {
private:

    VirtualTexture* texture;

    int id = 0;

    GameObject* parent = nullptr;

    std::string name;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::mat4> jointMatrix;
    std::vector<tinygltf::Node> jointsNode;
    std::vector<unsigned int> jointsNodeIndices;
protected:
    bool textureHasLoaded = false;

    glm::vec3 position = glm::vec3(0.0,0.0,0.0);
    glm::quat rotation = glm::quat(0.0,0.0,0.0,0.0);
    glm::vec3 scale = glm::vec3(1.0,1.0,1.0);
public:
    GameObject();
    virtual ~GameObject();
    virtual nlohmann::json GetJson();
    virtual std::vector<unsigned int> GetIndices();
    virtual std::vector<glm::mat4> GetJointMatrix();
    virtual std::vector<tinygltf::Node> GetJoints();
    virtual std::vector<unsigned int> GetJointsIndices();
    virtual std::vector<Vertex> GetMeshes();
    virtual void Action() = 0;
    virtual void LoadMeshes(tinygltf::Model model,tinygltf::Mesh* mesh,tinygltf::Skin* skin);
    glm::mat4 GetModelMatrix();
    VirtualTexture* GetTexture();
    void SetTexture(VirtualTexture* texture);
    bool GetTextureHasLoaded();

    void ProcessAnimation(tinygltf::Model model,tinygltf::Animation* animation);

    GameObject* GetParent();
    void SetParent(GameObject* obj);

    void SetID(int _id);
    int GetID();

    void SetName(std::string _name);
    std::string GetName();

    void SetPosition(glm::vec3 _position);
    glm::vec3& GetPosition();
    void SetRotation(glm::quat _position);
    glm::quat& GetRotation();
    void SetScale(glm::vec3 _position);
    glm::vec3& GetScale();
};

#endif //EUROPARENDER_GAMEOBJECT_H
