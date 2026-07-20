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
