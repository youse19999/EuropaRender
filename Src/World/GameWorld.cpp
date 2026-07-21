//
// Created by youse on 2026/07/19.
//

#include "GameWorld.h"

#include <imgui.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "logger.h"
#include "mat42str.h"

std::shared_ptr<GameWorld> GameWorld::instance{nullptr};
std::mutex GameWorld::m_singleton;

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
    gameObjectOffset = gameObjectLastOffset;
    std::shared_ptr<GameWorld> instance1(this);
    this->instance.swap(instance1);
}
/*
 *継承した結果です。
 */
void GameWorld::Dispose() {

}
void GameWorld::PollEvent() {

}
void GameWorld::Init() {

}

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
void GameWorld::Render() {
    /*
    Viewはビュー行列
    Projectionはカメラ行列です。
    Modelは、オブジェクトが個別に持ちます。
    */

    //Viewのシェダー部分を送信します。
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    //ここで、glm側のポインタを受け取り、viewLocのLocationに送信します。
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(curentCamera->GetViewMatrix()));
    //同様にProjectionを送信しています。
    GLint proejctionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(proejctionLoc, 1, GL_FALSE, glm::value_ptr(curentCamera->GetProjectionMatrix()));
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint jointMatrixLoc = glGetUniformLocation(shaderProgram, "jointMatrix");
    //ゲームのオブジェクトリストを保存します。

    /*
     *ここで、オフセットを変更しながら描画処理を回すことで、描画を分けます。
    */
    currentIndexOffset = 0;
    for (auto gameObject : gameObjects) {
        //TODO:texture1の名前をまともにする。
        if (gameObject->GetTextureHasLoaded()) {
            GLint textureUniformLocation = glGetUniformLocation(shaderProgram, "texture1");
            //テクスチャを送信する。
            glUniform1i(textureUniformLocation, 0);
            //テクスチャをアクティブにする。
            glActiveTexture(GL_TEXTURE0);
            //テクスチャを割り当てる。
            glBindTexture(GL_TEXTURE_2D, gameObject->GetTexture()->GetTextureID());
        }else {
            LOG_RENDER(":( Texture is null" << this);
            LOG_RENDER(":( Texture at " << gameObject->GetTexture());
        }
        //スキンメッシュを使うか
        GLint isSkinUniformLocation = glGetUniformLocation(shaderProgram, "isSkin");

        //TODO:フラグをアニメーションがあるかどうかにする。
        if (gameObject->GetJointMatrix().size() == 0) {
            glUniform1i(isSkinUniformLocation,0);
        }else {
            glUniform1i(isSkinUniformLocation,1);
        }
        std::vector<glm::mat4> inverseBindMatrix = gameObject->GetJointMatrix();
        std::vector<glm::mat4> jointMatrix;

        // スキニング用のジョイント行列計算部
        int i = 0;
        glm::mat4 invMeshModelMatrix = glm::inverse(gameObject->GetModelMatrix());
        for (auto index : gameObject->GetJointsIndices()) {
            auto obj = GameWorld::GetInstance()->GetGameObject(index);
            // 1. ルートからの累積モデル行列を取得
            glm::mat4 nodeWorldMatrix = CalculateNodeModelMatrix(obj);
            glm::mat4 nodeModelMatrix = invMeshModelMatrix * nodeWorldMatrix;
            // 2. ジョイント行列の計算（モデル空間行列 × インバースバインド行列）
            jointMatrix.push_back(nodeModelMatrix * inverseBindMatrix[i]);
            i = i + 1;
        }

        glUniformMatrix4fv(jointMatrixLoc, static_cast<GLsizei>(jointMatrix.size()), GL_FALSE, glm::value_ptr(jointMatrix[0]));

        //LOG_RENDER("JOINT MATRIX SIZE:" << gameObject->GetJointMatrix().size());
        //モデル
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gameObject->GetModelMatrix()));
        //描画処理
        LOG_RENDER("DRAW ARRA COUNT : "<< gameObject->GetIndices().size());
        glDrawElements(GL_TRIANGLES, gameObject->GetIndices().size(), GL_UNSIGNED_INT,(const void*)(currentIndexOffset * sizeof(GLuint)));
        currentIndexOffset += gameObject->GetIndices().size();
    }
}


GameObject *GameWorld::GetGameObject(unsigned int index) {
    GameObject* returnObject = nullptr;
    for (auto obj : gameObjects) {
        if (obj->GetID() == index) {
            return obj;
        }
    }
    return returnObject;
}

std::vector<GameObject *> GameWorld::GetAllGameObject() {
    return gameObjects;
}

GameCamera& GameWorld::GetCamera() {
    return *this->curentCamera;
}
std::shared_ptr<GameWorld> GameWorld::GetInstance()
{
    std::lock_guard<std::mutex> lock(m_singleton);
    return instance;
}
