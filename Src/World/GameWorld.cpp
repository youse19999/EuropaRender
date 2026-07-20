//
// Created by youse on 2026/07/19.
//

#include "GameWorld.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "logger.h"

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
/*
 *継承した結果です。
 */
void GameWorld::Dispose() {

}
void GameWorld::PollEvent() {

}
void GameWorld::Init() {

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
            LOG_RENDER(this << ">>MOCK LOG " << gameObject->GetTexture()->GetTextureID());
        }else {
            LOG_RENDER(":( Texture is null" << this);
            LOG_RENDER(":( Texture at " << gameObject->GetTexture());
        }
        std::vector<glm::mat4> jointMatrix = gameObject->GetJointMatrix();
        int i = 0;
        //TODO:これは機能しているが、RIGではないため、バグる。
        for (auto node : gameObject->GetJoints()) {
            glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(node.translation[0],node.translation[1],node.translation[2]));
            glm::mat4 R;
            if (node.rotation.size() != 0) {
                R = glm::toMat4(glm::quat(node.rotation[0],node.rotation[1],node.rotation[2],node.rotation[3]));
            }else {
                R = glm::toMat4(glm::quat(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
            }
            glm::mat4 S;
            if (node.scale.size() != 0) {
                S = glm::scale(glm::mat4(1.0f), glm::vec3(node.scale[0],node.scale[1],node.scale[2]));
            }else {
                S = glm::scale(glm::mat4(1.0f), glm::vec3(1,1,1));
            }

            glm::mat4 model = glm::mat4(1.0f) * T * R * S;

            jointMatrix[i] = model * jointMatrix[i];
            i = i + 1;
        }
        glUniformMatrix4fv(jointMatrixLoc, static_cast<GLsizei>(jointMatrix.size()), GL_FALSE, glm::value_ptr(jointMatrix[0]));

        LOG_RENDER("JOINT MATRIX SIZE:" << gameObject->GetJointMatrix().size());
        //モデル
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(gameObject->GetModelMatrix()));
        //描画処理
        LOG_RENDER("DRAW ARRA COUNT : "<< gameObject->GetIndices().size());
        glDrawElements(GL_TRIANGLES, gameObject->GetIndices().size(), GL_UNSIGNED_INT,(const void*)(currentIndexOffset * sizeof(GLuint)));
        currentIndexOffset += gameObject->GetIndices().size();
        //glDrawArrays(GL_TRIANGLES, 0, gameObject->GetMeshes().size());
    }
}

GameCamera& GameWorld::GetCamera() {
    return *this->curentCamera;
}
