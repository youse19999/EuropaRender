//
// Created by youse on 2026/07/20.
//

#ifndef EUROPARENDER_TREEOBJECT_H
#define EUROPARENDER_TREEOBJECT_H
#include <glm/mat4x4.hpp>

class TreeObject {
private:
    glm::mat4 localMatrix;
    glm::mat4 worldMatrix;
    TreeObject* parent;
    std::vector<TreeObject> children;
};
#endif //EUROPARENDER_TREEOBJECT_H
