//
// Created by youse on 2026/07/21.
//
#include <FKMath.h>
glm::mat4 CalculateNodeModelMatrix(GameObject* obj) {
    // 自身のローカルTRS行列を作成
    glm::mat4 T = glm::translate(glm::mat4(1.0f), obj->GetPosition());
    glm::mat4 R = glm::mat4_cast(obj->GetRotation());
    glm::mat4 S = glm::scale(glm::mat4(1.0f), obj->GetScale());
    glm::mat4 localMatrix = T * R * S;

    // ルートまで親を遡って行列を累積計算する

    GameObject* parent = obj->GetParent();
    if (parent != nullptr) {
        return CalculateNodeModelMatrix(parent) * localMatrix;
    }
    return localMatrix;
}