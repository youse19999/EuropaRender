//
// Created by youse on 2026/07/19.
//

#include <GameObject.h>

nlohmann::json GameObject::GetJson() {
    return nlohmann::json();
}

std::vector<glm::vec3> GameObject::GetMeshes() {
    return vertices;
}

GameObject::~GameObject() {

}